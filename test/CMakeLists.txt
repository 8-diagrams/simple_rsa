cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++11 -Wall -Wextra)
include_directories(${PROJECT_SOURCE_DIR})
link_directories(${CMAKE_BINARY_DIR})

add_executable(test_arithmetic test_arithmetic.cpp)
target_link_libraries(test_arithmetic mybiguint)

configure_file(test_arithmetic.py ${CMAKE_BINARY_DIR}/test/test_arithmetic.py)
