include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(awesomeCC_test strtools_test.cc token_test.cc)

target_link_libraries(awesomeCC_test gtest gtest_main)
target_link_libraries(awesomeCC_test awesomeCC_lib)

