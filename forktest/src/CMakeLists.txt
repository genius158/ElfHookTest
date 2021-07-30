# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_compile_options(-Oz -flto -ffunction-sections -fdata-sections -fexceptions -frtti)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -O3 -flto -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl,--no-fatal-warnings")

# Header file directories
include_directories(
)

# Source code directories
add_library(
        # Sets the name of the library.
        forktest

        # Sets the library as a shared library.
        SHARED

        ${CMAKE_SOURCE_DIR}/main/cpp/ForkTest.cpp
)


find_library(
        # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that you want CMake to locate.
        log
)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.


target_link_libraries(
        # Specifies the target library.
        forktest

        # Links the target library to the log library included in the NDK.
        #        -latomic
        ${log-lib}
#        native-lib
)