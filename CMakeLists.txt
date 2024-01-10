cmake_minimum_required(VERSION 3.28)
project(cpp-utilities)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)


#  ==============
#  ||  `args`  ||
#  ==============

add_executable(args.demo args/args.demo.cpp)
target_sources(args.demo
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES FILES
        args/args.cppm
)

add_executable(args.tests args/args.tests.cpp)
target_sources(args.tests
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES FILES
        args/args.cppm
        testing/testing.cppm
)
