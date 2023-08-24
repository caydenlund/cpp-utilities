cmake_minimum_required(VERSION 3.10)
project(cpp_utilities)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_RELEASE -O3)

include_directories(.)

add_executable(args.tests args/args.tests.cpp)

add_executable(thread_queue.tests thread_queue/thread_queue.tests.cpp)
