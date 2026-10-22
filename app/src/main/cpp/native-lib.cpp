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
