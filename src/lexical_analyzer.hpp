//
// Created by Keyi Li on 2018/11/10.
//


#ifndef TOY_C_COMPILER_LEXICAL_ANALYZER_HPP
#define TOY_C_COMPILER_LEXICAL_ANALYZER_HPP

#endif //TOY_C_COMPILER_LEXICAL_ANALYZER_HPP

#include <string>
#include <vector>
#include <iostream>
using namespace std;

#include "token.hpp"
#include "error.hpp"

class LexicalAnalyzer {
private:
    vector<Token> tokens;
    vector<Error> errors;
    string sentence;
    int curPos, curLineNumber, len;

    bool _isBlank();
    bool _isKeyword(string word);
    bool _isSeparator(char ch);
    bool _isOperator(char ch);

    void _skipBlank();
    void _init(string _sentence);
    void _analyze();


    void _DEBUG_();


public:
    LexicalAnalyzer();
    void analyze(string _sentence, int lineNumber = 0);
};