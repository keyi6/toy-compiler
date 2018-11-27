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
 *
 * @return Error object 返回一个错误对象
 */
Error::Error(string _errorMsg) {
    errorMsg = _errorMsg;
}

ostream & operator << (ostream & out, Error & e) {
    out << "[Error]" << e.errorMsg << endl;
    return out;
}
