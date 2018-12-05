/**
 *
 * @file token_test.cc
 * @brief 测试
 *
 * @author Keyi Li
 *
 */

#include "gtest/gtest.h"
#include "../../awesomeCC/lib/include/token.h"


TEST(isExpOpe_test, isExpOpe_test_case) {
    EXPECT_EQ(Token::isExpressionOperator(TOKEN_TYPE_ENUM::LL_BRACKET), true);
    EXPECT_EQ(Token::isExpressionOperator(TOKEN_TYPE_ENUM::RL_BRACKET), true);
    EXPECT_EQ(Token::isExpressionOperator(TOKEN_TYPE_ENUM::LB_BRACKET), false);
    EXPECT_EQ(Token::isExpressionOperator(TOKEN_TYPE_ENUM::PLUS), true);
    EXPECT_EQ(Token::isExpressionOperator(TOKEN_TYPE_ENUM::MINUS), true);
    EXPECT_EQ(Token::isExpressionOperator(TOKEN_TYPE_ENUM::MUL), true);
    EXPECT_EQ(Token::isExpressionOperator(TOKEN_TYPE_ENUM::DIV), true);
}