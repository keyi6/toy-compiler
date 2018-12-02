//
// Created by cj on 2018/12/2.
//

#ifndef AWESOMECC_STRTOOLS_H
#define AWESOMECC_STRTOOLS_H

#include <string>
#include <sstream>
using std::string;
using std::stringstream;


/**
 * @brief 将字符转化为字符串
 * @param ch char, 输入字符
 * @return string, 返回字符串
 */
string char2string(char ch) {
    stringstream ss;
    ss << ch;

    string ret = ss.str();
    ss.clear();

    return ret;
}


/**
 * @brief 将字符转化为int
 * @str string，输入字符串
 * @return int，返回int
 */
int string2int(string str) {
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
 * @str string，输入字符串
 * @return double，返回double
 */
 double string2double(string str) {
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


#endif //AWESOMECC_STRTOOLS_H
