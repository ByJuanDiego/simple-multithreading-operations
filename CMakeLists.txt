cmake_minimum_required(VERSION 3.23)
project(parallel_algorithms)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp parallel_container.cpp parallel_container.h result_t.h)
