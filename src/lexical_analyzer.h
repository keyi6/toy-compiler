/**
 *
 * @file lexical_analyzer.h
 * @brief 词法分析器类生成Token列表和Error列表
 *
 * @author Keyi Li
 *
 */
#ifndef TOY_C_COMPILER_LEXICAL_ANALYZER_HPP
#define TOY_C_COMPILER_LEXICAL_ANALYZER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "token.h"
#include "error.h"

class LexicalAnalyzer {
private:
    vector<Token> tokens, all_tokens;
    vector<Error> errors;
    string sentence;
    int cur_pos, cur_line_number, len;

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
    bool analyze(vector<string> _sentences, bool verbose = true);
};


#endif //TOY_C_COMPILER_LEXICAL_ANALYZER_HPP
