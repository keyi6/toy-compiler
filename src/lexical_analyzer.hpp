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

class LexicalAnalyzer {
private:
    vector<Token> tokens;
    string sentence;
    int curPos, len;

    bool _isBlank();
    bool _isKeywords(string word);
    void _skipBlank();
    void _init(string _sentence);
    void _analyze();


    void _OUTPUT_TOKENS();


public:
    LexicalAnalyzer();
    void analyze(string _sentence);
};