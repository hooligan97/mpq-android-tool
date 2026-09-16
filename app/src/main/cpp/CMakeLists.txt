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
