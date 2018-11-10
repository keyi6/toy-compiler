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
public:
    string value;
    int typeIndex;

    Token();
    Token(string _value, int _typeIndex);

    static vector<string> TOKEN_TYPE; // Token 种类
    static map<string, string> DETAIL_TOKEN_STYLE; // 将关键字、运算符、分隔符进行具体化
    static vector<string> KEYWORDS; // 关键字
    static vector<string> OPERATORS; // 运算符
    static vector<string> DELIMITERS; // 分隔符
};



