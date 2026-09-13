# Top-level CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(mpq-android-tool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Architecture detection
if(ANDROID_ABI STREQUAL "armeabi-v7a")
    message(STATUS "Building for armeabi-v7a")
elseif(ANDROID_ABI STREQUAL "armeabi")
    message(STATUS "Building for armeabi")
elseif(ANDROID_ABI STREQUAL "x86")
    message(STATUS "Building for x86")
endif()

# Add subdirectories
add_subdirectory(mpq-lib)
