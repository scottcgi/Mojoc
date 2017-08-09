# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c99")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(
    src/main/cpp/
)

add_library(
    png STATIC

    ../../Code/pngget.c
    ../../Code/pngread.c
    ../../Code/pngrutil.c
    ../../Code/pngtrans.c
    ../../Code/pngwtran.c
    ../../Code/png.c
    ../../Code/pngmem.c
    ../../Code/pngrio.c
    ../../Code/pngset.c
    ../../Code/pngwio.c
    ../../Code/pngwutil.c
    ../../Code/pngerror.c
    ../../Code/pngpread.c
    ../../Code/pngrtran.c
    ../../Code/pngwrite.c
)

set_target_properties(
    png
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/${ANDROID_ABI}"
)

target_link_libraries(
    png
    z
)

add_library(
    native-lib SHARED
    src/main/cpp/native-lib.cpp
)

target_link_libraries(
    native-lib
    png
)