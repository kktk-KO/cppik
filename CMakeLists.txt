cmake_minimum_required(VERSION 2.8)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)
# add_subdirectory(example)
add_subdirectory(test)
