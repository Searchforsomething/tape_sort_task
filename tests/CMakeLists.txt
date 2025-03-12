cmake_minimum_required(VERSION 3.30)
project(tape_tests)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tape_tests
        FileTapeTest.cpp
)

target_link_libraries(tape_tests PRIVATE GTest::GTest GTest::Main tape)

