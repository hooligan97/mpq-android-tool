#include <jni.h>
#include <string>
#include <android/log.h>

#define LOG_TAG "MPQToolNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jstring JNICALL
Java_com_hooligan_mpqtool_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "MPQ Native Engine Initialized Successfully";
    LOGI("Native JNI bridge initialized.");
    return env->NewStringUTF(hello.c_str());
}
