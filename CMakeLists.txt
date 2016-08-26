cmake_minimum_required(VERSION 3.2)

add_subdirectory(test)

find_package(Boost COMPONENTS program_options REQUIRED)

add_compile_options (-std=c++11 -Wall -Wextra)
include_directories (${PROJECT_SOURCE_DIR})

add_executable(simple_rsa simple_rsa.cpp)
target_link_libraries(simple_rsa ${Boost_LIBRARIES})
