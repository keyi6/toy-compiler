cmake_minimum_required(VERSION 3.2)

PROJECT(toy_c_compiler)

INCLUDE_DIRECTORIES(src)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)


ADD_EXECUTABLE(${PROJECT_NAME} src/main.cpp src/token.hpp)
