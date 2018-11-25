cmake_minimum_required(VERSION 3.2)

PROJECT(toy_c_compiler)

add_definitions(-std=c++11)

INCLUDE_DIRECTORIES(src)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)


ADD_EXECUTABLE(${PROJECT_NAME} src/main.cc src/token.h src/lexical_analyzer.cc src/lexical_analyzer.h src/token.cc src/error.cc src/error.h src/syntax_analyzer.cc src/syntax_analyzer.h)


