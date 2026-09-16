#!/bin/bash
set -e

echo "1. Dodawanie submodule StormLib..."
if [ ! -d "app/src/main/cpp/StormLib" ]; then
    git submodule add https://github.com/ladislav-zezula/StormLib.git app/src/main/cpp/StormLib || true
fi
git submodule update --init --recursive

echo "2. Aktualizacja CMakeLists.txt..."
cat << 'EOT' > app/src/main/cpp/CMakeLists.txt
cmake_minimum_required(VERSION 3.22.1)

project("mpqtool")

set(STORM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(STORM_USE_BUNDLED_LIBRARIES ON CACHE BOOL "" FORCE)
set(WITH_BUNDLED_LIBTOMCRYPT ON CACHE BOOL "" FORCE)
set(WITH_BUNDLED_LIBTOMMATH ON CACHE BOOL "" FORCE)

add_subdirectory(StormLib)

add_library(
    mpqtool
    SHARED
    native-lib.cpp
)

target_include_directories(mpqtool PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/StormLib/src
)

find_library(
    log-lib
    log
)

target_link_libraries(
    mpqtool
    StormLib
    ${log-lib}
)
EOT

echo "3. Aktualizacja native-lib.cpp..."
cat << 'EOT' > app/src/main/cpp/native-lib.cpp
#include <jni.h>
#include <string>
#include <vector>
#include <android/log.h>
#include <StormLib.h>

#define LOG_TAG "StormLibJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jboolean JNICALL
Java_com_hooligan_mpqtool_MainActivity_extractMpqFile(
        JNIEnv* env,
        jobject /* this */,
        jstring mpqPathStr,
        jstring fileNameStr,
        jstring outputPathStr) {

    const char* mpqPath = env->GetStringUTFChars(mpqPathStr, nullptr);
    const char* fileName = env->GetStringUTFChars(fileNameStr, nullptr);
    const char* outputPath = env->GetStringUTFChars(outputPathStr, nullptr);

    HANDLE hArchive = NULL;
    HANDLE hFile = NULL;
    bool success = false;

    if (!SFileOpenArchive(mpqPath, 0, STREAM_FLAG_READ_ONLY, &hArchive)) {
        LOGE("Failed to open MPQ archive: %s", mpqPath);
        goto cleanup;
    }

    if (!SFileOpenFileEx(hArchive, fileName, 0, &hFile)) {
        LOGE("Failed to open file in MPQ: %s", fileName);
        goto cleanup;
    }

    {
        DWORD fileSize = SFileGetFileSize(hFile, NULL);
        if (fileSize == STORM_INVALID_SIZE) {
            LOGE("Failed to get file size for: %s", fileName);
            goto cleanup;
        }

        std::vector<char> buffer(fileSize);
        DWORD bytesRead = 0;

        if (!SFileReadFile(hFile, buffer.data(), fileSize, &bytesRead, NULL) || bytesRead != fileSize) {
            LOGE("Failed to read file content: %s", fileName);
            goto cleanup;
        }

        FILE* outFile = fopen(outputPath, "wb");
        if (!outFile) {
            LOGE("Failed to open output file: %s", outputPath);
            goto cleanup;
        }

        fwrite(buffer.data(), 1, bytesRead, outFile);
        fclose(outFile);
        LOGI("Successfully extracted %s to %s", fileName, outputPath);
        success = true;
    }

cleanup:
    if (hFile) SFileCloseFile(hFile);
    if (hArchive) SFileCloseArchive(hArchive);

    env->ReleaseStringUTFChars(mpqPathStr, mpqPath);
    env->ReleaseStringUTFChars(fileNameStr, fileName);
    env->ReleaseStringUTFChars(outputPathStr, outputPath);

    return success ? JNI_TRUE : JNI_FALSE;
}
EOT

echo "4. Aktualizacja MainActivity.kt..."
cat << 'EOT' > app/src/main/java/com/hooligan/mpqtool/MainActivity.kt
package com.hooligan.mpqtool

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val textView: TextView = findViewById(R.id.sample_text)
        textView.text = "StormLib Native Engine Ready"
    }

    external fun extractMpqFile(mpqPath: String, fileName: String, outputPath: String): Boolean

    companion object {
        init {
            System.loadLibrary("mpqtool")
        }
    }
}
EOT

echo "5. Wysyłanie zmian na GitHub..."
git add .
git commit -m "Integrate StormLib C++ library with JNI extraction method" || true
git push origin main

echo "Gotowe! StormLib został podpięty i wysłany na GitHub."
