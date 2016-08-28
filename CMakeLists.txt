cmake_minimum_required(VERSION 3.2)

find_package(Boost COMPONENTS program_options REQUIRED)

add_compile_options(-std=c++11 -Wall -Wextra)
include_directories(${PROJECT_SOURCE_DIR})

add_library(mybiguint STATIC biguint.cpp)

add_library(mysra STATIC rsa.cpp)
target_link_libraries(mysra mybiguint)

add_executable(simple_rsa simple_rsa.cpp)
target_link_libraries(simple_rsa mysra ${Boost_LIBRARIES})

add_subdirectory(test)
