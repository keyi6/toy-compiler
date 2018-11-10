//
// Created by Keyi Li on 2018/11/10.
//


#include "token.hpp"

Token::Token() {}

Token::Token(string _value, string _typeIndex) {
    value = _value;
    typeIndex = _typeIndex;
}

string Token::TOKEN_STYLE[] = {"KEY_WORD", "IDENTIFIER", "DIGIT_CONSTANT",
                               "OPERATOR", "SEPARATOR", "STRING_CONSTANT"};

map<string, string> Token::DETAIL_TOKEN_STYLE = {
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

string Token::KEYWORDS[][5] = {
        {"int", "float", "double", "char", "void"},
        {"if", "for", "while", "do", "else"},
        {"include", "return"}
};


string Token::OPERATORS[] = {"=", "&", "<", ">", "++", "--", "+", "-", "*", "/", ">=", "<=", "!=" };


string Token::DELIMITERS[] = {"(", ")", "{", "}", "[", "]", ",", "\"", ";" };
