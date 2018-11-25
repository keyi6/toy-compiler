/**
 *
 * @file lexical_analyzer.hpp
 * @brief 词法分析器类生成Token列表和Error列表
 *
 * @author Keyi Li
 *
 */
#ifndef TOY_C_COMPILER_LEXICAL_ANALYZER_HPP
#define TOY_C_COMPILER_LEXICAL_ANALYZER_HPP

#endif //TOY_C_COMPILER_LEXICAL_ANALYZER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "token.hpp"
#include "error.hpp"

class LexicalAnalyzer {
private:
    vector<Token> tokens, allTokens;
    vector<Error> errors;
    string sentence;
    int curPos, curLineNumber, len;

    string _char2string(char ch);

    bool _isBlank();
    bool _isKeyword(string word);
    bool _isSeparator(char ch);
    bool _isOperator(char ch);

    void _skipBlank();
    void _init(string _sentence);
    void _analyze();

public:
    LexicalAnalyzer();
    vector<Token> getAllTokens();
    vector<Error> getAllError();
    bool analyzeSentence(string _sentence);
    bool analyze(vector<string> _sentences);
};