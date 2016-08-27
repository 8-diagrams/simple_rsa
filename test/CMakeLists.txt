cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++11 -Wall -Wextra)
include_directories(${PROJECT_SOURCE_DIR})
link_directories(${CMAKE_BINARY_DIR})

add_executable(test_add_sub test_add_sub.cpp)
target_link_libraries(test_add_sub -lmysra)
configure_file(test_add_sub.py ${CMAKE_BINARY_DIR}/test/test_add_sub.py)
