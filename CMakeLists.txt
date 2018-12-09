cmake_minimum_required(VERSION 3.2)

PROJECT(awesomeCC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(awesomeCC)
#add_subdirectory(awesomeCC_test)

#target_link_libraries(awesomeCC_run awesomeCC)
