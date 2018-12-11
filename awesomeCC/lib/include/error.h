/**
 * @file error.h
 * @brief 错误类，用于报错
 */

#ifndef AWESOMECC_ERROR_HPP
#define AWESOMECC_ERROR_HPP

#include <string>
using std::string;
using std::ostream;


class Error {
private:
    int line_number; // 行号
    int pos;         // 行中位置
    string errorMsg;

public:
    Error(string _errorMsg, int _line_number = -1, int _pos = -1);

    friend ostream & operator << (ostream & out, Error & e);
};


#endif //AWESOMECC_ERROR_HPP
