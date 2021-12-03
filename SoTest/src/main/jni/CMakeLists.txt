cmake_minimum_required(VERSION 3.10.2)

project(dlsymtest)

MESSAGE("curpath ${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(.)

add_library(dlsymtest
        SHARED

        DlsymTest.cpp
        DlsymTestCall.cpp
        DlsymEntryPoint.cpp
        )
#target_compile_options(sample PUBLIC -Weverything -Werror)
target_link_libraries(dlsymtest log)
