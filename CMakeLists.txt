cmake_minimum_required(VERSION 3.2)

PROJECT(awesomeCC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/test)


file(GLOB SOURCES "src/*.cc")
file(GLOB HEADERS "include/*.h")
file(GLOB TESTFILES "test/*.cc")


# 编译成库
add_library(awesomecc_lib ${HEADERS} ${SOURCES} )

# 编译成可执行文件
add_executable(awesomeCC app/main.cc ${HEADERS} ${SOURCES} )


