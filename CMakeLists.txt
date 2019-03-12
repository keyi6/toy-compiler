cmake_minimum_required(VERSION 3.2)

# over all project
PROJECT(llcc)

# compile args
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra ")

# source code
add_subdirectory(llcc)

