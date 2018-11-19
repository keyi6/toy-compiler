cmake_minimum_required(VERSION 3.2)

PROJECT(toy_c_compiler)

add_definitions(-std=c++11)

INCLUDE_DIRECTORIES(src)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)


ADD_EXECUTABLE(${PROJECT_NAME} src/main.cpp src/token.hpp src/lexical_analyzer.cpp src/lexical_analyzer.hpp src/token.cpp src/error.cpp src/error.hpp src/syntax_analyzer.cpp src/syntax_analyzer.hpp)


