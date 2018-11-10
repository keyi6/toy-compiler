//
// Created by Keyi Li on 2018/11/10.
//


#include "token.hpp"

Token::Token() {}

Token::Token(string _value, int _typeIndex, int _pos) {
    value = _value;
    typeIndex = _typeIndex;
    pos = _pos;
}

vector<string> Token::TOKEN_TYPE= {"KEYWORD", "IDENTIFIER", "DIGIT_CONSTANT",
                                   "OPERATOR", "SEPARATOR", "STRING_CONSTANT"};

// TODO: complete with all keywords
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

vector<string> Token::KEYWORDS = {
        "int", "float", "double", "char", "void",
        "if", "for", "while", "do", "else",
        "include", "return"
};


vector<string> Token::OPERATORS = {"+",  "-",  "<",  ">", "!", "=", "|", "&", "*", "/",  // 0 - 9
                                   "++", "--", "<<", ">>",         // 10 - 13
                                               "<=", ">=", "!="};  // 14 - 16



vector<char> Token::SEPARATORS = {'(', ')', '{', '}', '[', ']', ',', '\'', ';' };
