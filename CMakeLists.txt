cmake_minimum_required(VERSION 3.30)
project(tape_sort_task)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tape_sort_task src/main.cpp
        src/TapeInterface.h
        src/FileTape.h
        src/FileTape.cpp
        src/ConfigParser.cpp
        src/ConfigParser.h
        src/TapeSorter.cpp
        src/TapeSorter.h
#        tests/test_main.cpp
#        tests/TestConfigParser.h
)
