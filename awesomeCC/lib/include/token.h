/**
 * @file token.h
 * @brief Token类，用于识别Token
 *
 * @author Keyi Li
 *
 */
#ifndef AWESOMECC_TOKEN_HPP
#define AWESOMECC_TOKEN_HPP

#include <map>
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;
using std::map;
using std::ostream;


enum class TOKEN_TYPE_ENUM {
    KEYWORD = 0,
    IDENTIFIER,
    DIGIT_CONSTANT,
    OPERATOR,
    SEPARATOR,
    STRING_CONSTANT,
    /* detailed keyword type */
    INCLUDE,
    VOID,
    CLASS,
    PRINT,
    INT,
    FLOAT,
    CHAR,
    DOUBLE,
    FOR,
    IF,
    ELSE,
    WHILE,
    DO,
    RETURN,
    /* detailed operator type */
    // 按优先级排序的
    ASSIGN,
    // 优先级0
    AND,
    OR,
    LT,
    GT,
    GET,
    LET,
    EQUAL,
    NOT_EQUAL,
    // 优先级1
    PLUS,
    MINUS,
    // 优先级2
    MUL,
    MOD,
    DIV,
    // 优先级3
    RIGHT_SHIFT,
    LEFT_SHIFT,
    SELF_PLUS,
    SELF_MINUS,
    NOT,
    /* detailed separator type */
    LL_BRACKET,
    RL_BRACKET,
    LB_BRACKET,
    RB_BRACKET,
    LM_BRACKET,
    RM_BRACKET,
    COMMA,
    DOUBLE_QUOTE,
    SINGLE_QUOTE,
    SEMICOLON,
    SHARP,
    /* none type */
    NONE
};


/**
 * @brief Token 类
 */
class Token {
public:
    string value;
    int pos;
    TOKEN_TYPE_ENUM type;

    Token(string _value = "", TOKEN_TYPE_ENUM = TOKEN_TYPE_ENUM::NONE, int _pos = 0);

    static vector<string> TOKEN_TYPE;             // Token 种类
    static map<string, TOKEN_TYPE_ENUM> DETAIL_TOKEN_TYPE; // 将关键字、运算符、分隔符进行具体化
    static vector<string> KEYWORDS;               // 关键字
    static vector<string> OPERATORS;              // 运算符
    static vector<char> SEPARATORS;               // 分隔符

    static bool isExpressionOperator(TOKEN_TYPE_ENUM t);
    static bool isBoolOperator(TOKEN_TYPE_ENUM t);
    static bool isUniOperator(TOKEN_TYPE_ENUM t);

    friend ostream & operator << (ostream &out, Token & t);
};


#endif //AWESOMECC_TOKEN_HPP
