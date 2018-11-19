/**
 *
 * @file error.hpp
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
public:
    string errorMsg;

    Error(string _errorMsg);
};


#endif //TOY_C_COMPILER_ERROR_HPP
