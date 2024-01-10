cmake_minimum_required(VERSION 3.28)
project(cpp-utilities)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_executable(test.args)
target_sources(test.args
        PUBLIC
        args/args.tests.cpp
)
target_sources(test.args
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES FILES
        args/args.cppm
        testing/testing.cppm
)
