/**
 *
 * @file error.cc
 * @brief 错误类具体实现
 */

#include "../include/error.h"

#include <iostream>
using std::ostream;
using std::endl;


/**
 * @brief Error类构造函数
 * @param _errorMsg string, 报错信息
 * @param _line_number int, 行号，默认-1
 * @return Error object 返回一个错误对象
 */
Error::Error(string _errorMsg, int _line_number, int _pos) {
    errorMsg = _errorMsg;
    line_number = _line_number;
    pos = _pos;
}


/**
 * @brief 重载输出流
 */
ostream & operator << (ostream & out, Error & e) {
    if (~ e.line_number)
        out << "On line " << e.line_number << " ";
    if (~ e.pos)
        out << "pos " << e.pos << " ";
    if (~ e.line_number || ~ e.pos)
        out << ": ";

    out << e.errorMsg << endl;
    return out;
}
