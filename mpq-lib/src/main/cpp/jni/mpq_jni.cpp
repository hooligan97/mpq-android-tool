#include <jni.h>
#include "../include/mpq_reader.h"
#include "../include/mpq_writer.h"
#include "../include/mpq_search.h"
#include <android/log.h>
#include <string>
#include <vector>

#define LOG_TAG "MPQ-JNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" {

java_package = "com/mpq/android/tool";

// MPQ Reader JNI Methods
JNIEXPORT jlong JNICALL Java_com_mpq_android_tool_MPQReader_nativeOpenMPQ
    (JNIEnv* env, jobject thiz, jstring filepath) {
    const char* path = env->GetStringUTFChars(filepath, nullptr);
    
    auto reader = new mpq::MPQReader(path);
    if (reader->open()) {
        LOGI("Opened MPQ file: %s", path);
        env->ReleaseStringUTFChars(filepath, path);
        return reinterpret_cast<jlong>(reader);
    } else {
        LOGE("Failed to open MPQ file: %s", path);
        env->ReleaseStringUTFChars(filepath, path);
        delete reader;
        return 0;
    }
}

JNIEXPORT void JNICALL Java_com_mpq_android_tool_MPQReader_nativeCloseMPQ
    (JNIEnv* env, jobject thiz, jlong handle) {
    auto reader = reinterpret_cast<mpq::MPQReader*>(handle);
    if (reader) {
        reader->close();
        delete reader;
    }
}

JNIEXPORT jobjectArray JNICALL Java_com_mpq_android_tool_MPQReader_nativeListFiles
    (JNIEnv* env, jobject thiz, jlong handle) {
    auto reader = reinterpret_cast<mpq::MPQReader*>(handle);
    if (!reader) return nullptr;

    auto files = reader->list_files();
    jobjectArray result = env->NewObjectArray(
        files.size(),
        env->FindClass("java/lang/String"),
        env->NewStringUTF(""));

    for (size_t i = 0; i < files.size(); ++i) {
        env->SetObjectArrayElement(result, i,
            env->NewStringUTF(files[i].filename.c_str()));
    }

    return result;
}

JNIEXPORT jboolean JNICALL Java_com_mpq_android_tool_MPQReader_nativeExtractFile
    (JNIEnv* env, jobject thiz, jlong handle, jstring filename, jstring output) {
    auto reader = reinterpret_cast<mpq::MPQReader*>(handle);
    if (!reader) return JNI_FALSE;

    const char* fname = env->GetStringUTFChars(filename, nullptr);
    const char* out = env->GetStringUTFChars(output, nullptr);

    bool result = reader->extract_file(fname, out);

    env->ReleaseStringUTFChars(filename, fname);
    env->ReleaseStringUTFChars(output, out);

    return result ? JNI_TRUE : JNI_FALSE;
}

// MPQ Search JNI Methods
JNIEXPORT jobjectArray JNICALL Java_com_mpq_android_tool_MPQSearch_nativeSearchByName
    (JNIEnv* env, jobject thiz, jlong handle, jstring pattern) {
    auto reader = reinterpret_cast<mpq::MPQReader*>(handle);
    if (!reader) return nullptr;

    mpq::MPQSearch searcher(reader);
    const char* pat = env->GetStringUTFChars(pattern, nullptr);
    
    auto results = searcher.search_by_name(pat);
    env->ReleaseStringUTFChars(pattern, pat);

    jobjectArray result = env->NewObjectArray(
        results.size(),
        env->FindClass("java/lang/String"),
        env->NewStringUTF(""));

    for (size_t i = 0; i < results.size(); ++i) {
        env->SetObjectArrayElement(result, i,
            env->NewStringUTF(results[i].filename.c_str()));
    }

    return result;
}

} // extern "C"
