//
// Created by cj on 2018/11/27.
//

#include "gtest/gtest.h"
#include "../../awesomeCC/include/str_tools.h"

TEST(string2int_test, string2int_test_case) {
    EXPECT_EQ(string2int("1123"), 1123);
    EXPECT_EQ(string2int("-991303"), -991303);
}


TEST(string2double_test, string2int_test_case) {
    EXPECT_EQ(string2double("11.5"), 11.5);
    EXPECT_EQ(string2double("-10.125"), -10.125f);
}

TEST(char2string_test, char2string_test_case) {
    EXPECT_EQ(char2string('A'), "A");
    EXPECT_EQ(char2string('c'), "c");
}

