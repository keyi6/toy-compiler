/**
 *
 * @file lexical_analyzer.h
 * @brief 词法分析器类生成Token列表和Error列表
 *
 * @author Keyi Li
 *
 */
#ifndef AWESOME_CC_LEXICAL_ANALYZER_HPP
#define AWESOME_CC_LEXICAL_ANALYZER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "token.h"
#include "error.h"
using namespace std;


class LexicalAnalyzer {
private:
    vector<Token> tokens, all_tokens;
    vector<int> line_number_map;
    string sentence;
    int cur_pos, cur_line_number, len;

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
    vector<int> getLineNumberMap();
    bool analyze(vector<string> _sentences, bool verbose = true);
};


#endif //AWESOME_CC_LEXICAL_ANALYZER_HPP
