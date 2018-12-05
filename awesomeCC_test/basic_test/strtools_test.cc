/**
 *
 * @file strtools_test.cc
 * @brief 测试
 *
 * @author Keyi Li
 *
 */

#include "gtest/gtest.h"
#include "../../awesomeCC/lib/include/str_tools.h"
#include "../../awesomeCC/lib/include/token.h"


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


TEST(token2string_test, token2string_test_case) {
    EXPECT_EQ(token2string(TOKEN_TYPE_ENUM::SELF_MINUS), "--" );
    EXPECT_EQ(token2string(TOKEN_TYPE_ENUM::LL_BRACKET), "(");
}
