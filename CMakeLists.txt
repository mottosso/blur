cmake_minimum_required (VERSION 2.8)
project (BLUR)

# Version
set (BLUR_VERSION_MAJOR 0)
set (BLUR_VERSION_MINOR 1)
set (BLUR_VERSION_PATCH 0)

include_directories(
    "src"
    "ext"
)

file(GLOB SOURCES "src/*.c")

add_executable(blur ${SOURCES})

find_library(M_LIB m)
target_link_libraries(blur ${M_LIB})