/**
 *
 * @file token.hpp
 * @brief Token类，用于识别Token
 *
 * @author Keyi Li
 *
 */
#ifndef TOY_C_COMPILER_TOKEN_HPP
#define TOY_C_COMPILER_TOKEN_HPP

#endif //TOY_C_COMPILER_TOKEN_HPP

#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;


// Token 类
class Token {
public:
    string value;
    int typeIndex, pos;

    Token();
    Token(string _value, int _typeIndex, int _pos = 0);

    static vector<string> TOKEN_TYPE; // Token 种类
    static map<string, string> DETAIL_TOKEN_STYLE; // 将关键字、运算符、分隔符进行具体化
    static vector<string> KEYWORDS; // 关键字
    static vector<string> OPERATORS; // 运算符
    static vector<char> SEPARATORS; // 分隔符
};

