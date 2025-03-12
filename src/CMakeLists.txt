cmake_minimum_required(VERSION 3.10)
project(tape)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS program_options)

add_library(tape STATIC
        TapeInterface.h
        FileTape.h
        FileTape.cpp
        ConfigParser.cpp
        ConfigParser.h
        TapeSorter.cpp
        TapeSorter.h
)

target_include_directories(tape PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(tape PRIVATE Boost::program_options)


add_executable(tape_exec main.cpp)
target_link_libraries(tape_exec PRIVATE tape Boost::program_options)
