cmake_minimum_required(VERSION 3.2)

add_compile_options (-std=c++11 -Wall -Wextra)
include_directories (${PROJECT_SOURCE_DIR})

add_executable(test_add_sub test_add_sub.cpp)
