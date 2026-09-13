cmake_minimum_required(VERSION 3.22.1)
project(mpq-core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(ZLIB REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/stormlib/src
)

# Architecture-specific compilation
message(STATUS "Compiling for Android ABI: ${ANDROID_ABI}")
if(ANDROID_ABI STREQUAL "armeabi-v7a")
    message(STATUS "Building for armeabi-v7a (32-bit ARM v7)")
    add_compile_options(-march=armv7-a -mfpu=neon)
elseif(ANDROID_ABI STREQUAL "armeabi")
    message(STATUS "Building for armeabi (legacy 32-bit ARM)")
    add_compile_options(-march=armv5te)
elseif(ANDROID_ABI STREQUAL "x86")
    message(STATUS "Building for x86 (32-bit Intel)")
    add_compile_options(-march=i686)
endif()

# MPQ Core Library
add_library(mpq-core SHARED
    mpq-core/mpq_reader.cpp
    mpq-core/mpq_writer.cpp
    mpq-core/mpq_search.cpp
    mpq-core/mpq_parser.cpp
    jni/mpq_jni.cpp
)

# Link libraries
target_link_libraries(mpq-core
    android
    log
    z
)

# Set library output directory
set_target_properties(mpq-core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs/${ANDROID_ABI}"
)
