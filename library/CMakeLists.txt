# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#add_compile_options(
#        -std=c11
#        -Weverything
#        -Wl
#)
#add_compile_options(-Oz -flto -ffunction-sections -fdata-sections -fexceptions -frtti)
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -O3 -flto -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl,--no-fatal-warnings")

# Header file directories
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/inline32
        ${CMAKE_SOURCE_DIR}/src/main/inline64
        ${CMAKE_SOURCE_DIR}/src/main/Substrate
        ${CMAKE_SOURCE_DIR}/src/main/cpp
        ${CMAKE_SOURCE_DIR}/src/main/xDL
        ${CMAKE_SOURCE_DIR}/src/main/xHook
        ${CMAKE_SOURCE_DIR}/src/main/xuwind
        ${CMAKE_SOURCE_DIR}/src/main/xuwind/include
)

if (${CMAKE_ANDROID_ARCH_ABI} STREQUAL "arm64-v8a")
    SET(
            # Sets the name of the library.
            PLATFORM_DIFF_CODE

            # And64InlineHook
            src/main/inline64/And64InlineHook.cpp
    )
else ()
    SET(
            # Sets the name of the library.
            PLATFORM_DIFF_CODE

            # Android-Inline-Hook
            src/main/inline32/relocate.c
            src/main/inline32/inlineHook.c

            # Substrate
            src/main/Substrate/hde64.c
            src/main/Substrate/SubstrateDebug.cpp
            src/main/Substrate/SubstrateHook.cpp
            src/main/Substrate/SubstratePosixMemory.cpp

    )
endif ()

# Source code directories
add_library(
        # Sets the name of the library.
        yhook

        # Sets the library as a shared library.
        SHARED

        # platform diff code
        ${PLATFORM_DIFF_CODE}

        src/main/cpp/Hook.cpp
        src/main/cpp/SoTool.c
#        src/main/cpp/HookThreadProxy2.c

        # xDL
        src/main/xDL/xdl.c
        src/main/xDL/xdl_iterate.c
        src/main/xDL/xdl_lzma.c
        src/main/xDL/xdl_util.c

        #xHook
        src/main/xhook/xh_core.c
        src/main/xhook/xh_elf.c
        src/main/xhook/xh_util.c
        src/main/xhook/xh_log.c
        src/main/xhook/xh_version.c
        src/main/xhook/xhook.c

        #xuwind
        src/main/xuwind/xu_cfi.c
        src/main/xuwind/xu_eh.c
        src/main/xuwind/xu_formatter.c
        src/main/xuwind/xu_fp_arm64.c
        src/main/xuwind/xu_fp_unimplemented.c
        src/main/xuwind/xu_jni.c
        src/main/xuwind/xu_libbacktrace.c
        src/main/xuwind/xu_libcorkscrew.c
        src/main/xuwind/xu_libcxx.c
        src/main/xuwind/xu_printer.c
        src/main/xuwind/xu_util.c
        src/main/xuwind/xunwind.c
        src/main/xuwind/xunwind_ctor.c
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

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
        yhook
        # Links the target library to the log library included in the NDK.
        #        -latomic
        ${log-lib}

)