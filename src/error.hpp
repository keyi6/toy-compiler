//
// Created by Keyi Li on 2018/11/10.
//

#ifndef TOY_C_COMPILER_ERROR_HPP
#define TOY_C_COMPILER_ERROR_HPP

#include <string>
using namespace std;


class Error {
private:
    int lineNumber;
    string errorMsg;

public:
    Error(int _lineNumber, string _errorMsg);
    void output();
};


#endif //TOY_C_COMPILER_ERROR_HPP
