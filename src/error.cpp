//
// Created by Keyi Li on 2018/11/10.
//


#include "error.hpp"

#include <iostream>
using namespace std;


Error::Error(int _lineNumber, string _errorMsg) {
    lineNumber = _lineNumber;
    errorMsg = _errorMsg;
}


void Error::output() {
    cout << "[Line " << lineNumber << "]" << endl;
    cout << "    " << errorMsg << endl;
}