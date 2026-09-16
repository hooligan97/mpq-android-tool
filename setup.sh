#!/bin/bash
set -e

# Tworzenie katalogów
mkdir -p app/src/main/java/com/hooligan/mpqtool
mkdir -p app/src/main/cpp
mkdir -p app/src/main/res/layout
mkdir -p app/src/main/res/values

# 1. settings.gradle
cat << 'EOT' > settings.gradle
rootProject.name = "mpq-android-tool"
include ':app'
EOT

# 2. build.gradle
cat << 'EOT' > build.gradle
buildscript {
    repositories {
        google()
        mavenCentral()
    }
    dependencies {
        classpath 'com.android.tools.build:gradle:8.1.1'
        classpath 'org.jetbrains.kotlin:kotlin-gradle-plugin:1.8.20'
    }
}

allprojects {
    repositories {
        google()
        mavenCentral()
    }
}

task clean(type: Delete) {
    delete rootProject.buildDir
}
EOT

# 3. app/build.gradle
cat << 'EOT' > app/build.gradle
plugins {
    id 'com.android.application'
    id 'org.jetbrains.kotlin.android'
}

android {
    namespace 'com.hooligan.mpqtool'
    compileSdk 33

    defaultConfig {
        applicationId "com.hooligan.mpqtool"
        minSdk 21
        targetSdk 33
        versionCode 1
        versionName "1.0"

        externalNativeBuild {
            cmake {
                cppFlags "-std=c++17"
                abiFilters "armeabi-v7a", "arm64-v8a", "x86", "x86_64"
            }
        }
    }

    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android-optimize.txt'), 'proguard-rules.pro'
        }
    }

    externalNativeBuild {
        cmake {
            path file("src/main/cpp/CMakeLists.txt")
            version "3.22.1"
        }
    }

    compileOptions {
        sourceCompatibility JavaVersion.VERSION_17
        targetCompatibility JavaVersion.VERSION_17
    }

    kotlinOptions {
        jvmTarget = '17'
    }
}

dependencies {
    implementation 'androidx.core:core-ktx:1.10.1'
    implementation 'androidx.appcompat:appcompat:1.6.1'
    implementation 'com.google.android.material:material:1.9.0'
    implementation 'androidx.constraintlayout:constraintlayout:2.1.4'
}
EOT

# 4. app/src/main/cpp/CMakeLists.txt
cat << 'EOT' > app/src/main/cpp/CMakeLists.txt
cmake_minimum_required(VERSION 3.22.1)
project("mpqtool")

add_library(
    mpqtool
    SHARED
    native-lib.cpp
)

find_library(
    log-lib
    log
)

target_link_libraries(
    mpqtool
    ${log-lib}
)
EOT

# 5. app/src/main/cpp/native-lib.cpp
cat << 'EOT' > app/src/main/cpp/native-lib.cpp
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
EOT

# 6. app/src/main/AndroidManifest.xml
cat << 'EOT' > app/src/main/AndroidManifest.xml
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android">

    <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />

    <application
        android:allowBackup="true"
        android:label="MPQ Tool"
        android:supportsRtl="true"
        android:theme="@style/Theme.AppCompat.Light.DarkActionBar">
        <activity
            android:name=".MainActivity"
            android:exported="true">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>

</manifest>
EOT

# 7. app/src/main/java/com/hooligan/mpqtool/MainActivity.kt
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
        textView.text = stringFromJNI()
    }

    external fun stringFromJNI(): String

    companion object {
        init {
            System.loadLibrary("mpqtool")
        }
    }
}
EOT

# 8. app/src/main/res/layout/activity_main.xml
cat << 'EOT' > app/src/main/res/layout/activity_main.xml
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent">

    <TextView
        android:id="@+id/sample_text"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_centerInParent="true"
        android:text="Wczytywanie..."
        android:textSize="18sp" />

</RelativeLayout>
EOT

echo "Gotowe! Wszystkie pliki zostały wygenerowane."
