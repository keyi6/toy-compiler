/**
 *
 * @file error.h
 * @brief 错误类，用于报错
 *
 * @author Keyi Li
 *
 */

#ifndef AWESOMECC_ERROR_HPP
#define AWESOMECC_ERROR_HPP

#include <string>
using std::string;
using std::ostream;


class Error {
private:
    int line_number;
    string errorMsg;

public:
    Error(string _errorMsg, int _line_number = -1);

    friend ostream & operator << (ostream & out, Error & e);
};


#endif //AWESOMECC_ERROR_HPP
