/**
 * @file str_tools.h
 * @brief 处理字符串的函数们
 */

#ifndef LLCC_STRTOOLS_H
#define LLCC_STRTOOLS_H

#include "token.h"

#include <string>
#include <sstream>
using std::string;
using std::stringstream;


/**
 * @brief 将字符转化为字符串
 * @param ch char, 输入字符
 * @return string, 返回字符串
 */
inline string char2string(char ch) {
    stringstream ss;
    ss << ch;

    string ret = ss.str();
    ss.clear();

    return ret;
}


/**
 * @brief 将字符转化为int
 * @param string，输入字符串
 * @return int，返回int
 */
inline int string2int(string str) {
    int ret = 0, len = str.length(), i = 0;
    bool flag = true;

    if (str[i] == '-')
        flag = false, i ++;

    for (; i < len; i ++) {
        if ('0' <= str[i] && str[i] <= '9')
            ret = ret * 10  + str[i] - '0';
        else
            throw "It is not an interger";
    }

    return flag ? ret : -ret;
}


/**
 * @brief 将字符转化为double
 * @param string，输入字符串
 * @return double，返回double
 */
 inline double string2double(string str) {
    double ret = 0, base = 0.1;
    bool meet_dot = false, flag = true;
    int len = str.length(), i = 0;

    if (str[i] == '-')
        flag = false, i ++;

    for (; i < len; i ++) {
        if ('0' <= str[i] && str[i] <= '9') {
            if (meet_dot) {
                ret += (str[i] - '0') * base;
                base *= 0.1;
            }
            else
                ret = ret * 10  + str[i] - '0';
        }
        else if (str[i] == '.')
            meet_dot = true;
        else
            throw "It is not a interger";
    }

    return flag ? ret : -ret;
}


/*
 * @brief 将int转为字符穿
 * @param x, 输入int
 * @return string，输入字符串
 */
inline string int2string(int x) {
    stringstream ss;
    ss << x;

    string ret = ss.str();
    ss.clear();

    return ret;
}


/*
 * @brief 将int转为字符穿
 * @param x, 输入int
 * @return string，输入字符串
 */
inline string double2string(double x) {
    stringstream ss;
    ss << x;

    string ret = ss.str();
    ss.clear();

    return ret;
}


/**
 * @brief 将Token type化为string
 * @param type, TOKEN_TYPE_ENUM
 * @return string，返回string
 */
inline string token2string(TOKEN_TYPE_ENUM type) {
    if (int(type) < int(Token::TOKEN_TYPE.size()))
    return Token::TOKEN_TYPE[int(type)];

    for (auto iter = Token::DETAIL_TOKEN_TYPE.begin(); iter != Token::DETAIL_TOKEN_TYPE.end(); iter ++)
        if (iter -> second == type)
            return iter -> first;

    return "";
}


#endif //LLCC_STRTOOLS_H
