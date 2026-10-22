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
