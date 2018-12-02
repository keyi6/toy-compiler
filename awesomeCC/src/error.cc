/**
 *
 * @file error.cc
 * @brief 错误类具体实现
 *
 * @author Keyi Li
 *
 */

#include "../include/error.h"

#include <iostream>
using namespace std;


/**
 * @brief Error类构造函数
 * @param _errorMsg string, 报错信息
 * @param _line_number int, 行号，默认-1
 * @return Error object 返回一个错误对象
 */
Error::Error(string _errorMsg, int _line_number) {
    errorMsg = _errorMsg;
    line_number = _line_number;
}

ostream & operator << (ostream & out, Error & e) {
    if (~ e.line_number)
        out << "On line " << e.line_number << ": ";
    out << e.errorMsg << endl;
    return out;
}
