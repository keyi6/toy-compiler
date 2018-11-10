//
// Created by Keyi Li on 2018/11/10.
//

#ifndef TOY_C_COMPILER_TOKEN_HPP
#define TOY_C_COMPILER_TOKEN_HPP

#endif //TOY_C_COMPILER_TOKEN_HPP

#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Token 种类
extern const string TOKEN_STYLE[] = {
        "KEY_WORD", "IDENTIFIER", "DIGIT_CONSTANT",
        "OPERATOR", "SEPARATOR", "STRING_CONSTANT"
};

// 将关键字、运算符、分隔符进行具体化
extern map<string, string> DETAIL_TOKEN_STYLE = {
        {"include", "INCLUDE"},
        {"int", "INT"},
        {"float", "FLOAT"},
        {"char", "CHAR"},
        {"double", "DOUBLE"},
        {"for", "FOR"},
        {"if", "IF"},
        {"else", "ELSE"},
        {"while", "WHILE"},
        {"do", "DO"},
        {"return", "RETURN"},
        {"=", "ASSIGN"},
        {"&", "ADDRESS"},
        {"<", "LT"},
        {">", "GT"},
        {"++", "SELF_PLUS"},
        {"--", "SELF_MINUS"},
        {"+", "PLUS"},
        {"-", "MINUS"},
        {"*", "MUL"},
        {"/", "DIV"},
        {">=", "GET"},
        {"<=", "LET"},
        {"(", "LL_BRACKET"},
        {")", "RL_BRACKET"},
        {"{", "LB_BRACKET"},
        {"}", "RB_BRACKET"},
        {"[", "LM_BRACKET"},
        {"]", "RM_BRACKET"},
        {",", "COMMA"},
        {"\"", "DOUBLE_QUOTE"},
        {";", "SEMICOLON"},
        {"#", "SHARP"},
};

// 关键字
extern const string KEYWORDS[][5] = {
        {"int", "float", "double", "char", "void"},
        {"if", "for", "while", "do", "else"},
        {"include", "return"}
};

// 运算符
extern const string operators[] = {
        "=", "&", "<", ">", "++", "--", "+", "-", "*", "/", ">=", "<=", "!="
};

// 分隔符
extern const string delimiters[] = {"(", ")", "{", "}", "[", "]", ",", "\"", ";" };
