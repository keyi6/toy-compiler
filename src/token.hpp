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


// Token 类
class Token {
private:
    string value;
    string typeIndex;

public:
    Token();
    Token(string _value, string _typeIndex);

    static string TOKEN_STYLE[]; // Token 种类
    static map<string, string> DETAIL_TOKEN_STYLE; // 将关键字、运算符、分隔符进行具体化
    static string KEYWORDS[][5]; // 关键字
    static string OPERATORS[]; // 运算符
    static string DELIMITERS[]; // 分隔符
};



