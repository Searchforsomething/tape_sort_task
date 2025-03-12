cmake_minimum_required(VERSION 3.30)
project(tape_sort_task)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests)

add_custom_target(tape_sort_task ALL DEPENDS tape_exec tape_tests)