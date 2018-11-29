/**
 *
 * @file error.h
 * @brief 错误类，用于报错
 *
 * @author Keyi Li
 *
 */

#ifndef TOY_C_COMPILER_ERROR_HPP
#define TOY_C_COMPILER_ERROR_HPP

#include <string>
using namespace std;


class Error {
private:
    int line_number;
    string errorMsg;

public:
    Error(string _errorMsg, int _line_number = -1);

    friend ostream & operator << (ostream & out, Error & e);
};


#endif //TOY_C_COMPILER_ERROR_HPP
