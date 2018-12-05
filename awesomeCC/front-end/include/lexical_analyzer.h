/**
 *
 * @file lexical_analyzer.h
 * @brief 词法分析器类生成Token列表和Error列表
 *
 * @author Keyi Li
 *
 */

#ifndef AWESOMECC_LEXICAL_ANALYZER_HPP
#define AWESOMECC_LEXICAL_ANALYZER_HPP

#include "../../lib/include/token.h"
#include "../../lib/include/error.h"
#include "../../lib/include/str_tools.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using std::string;
using std::cout;
using std::endl;
using std::vector;


class LexicalAnalyzer {
private:
    vector<Token> tokens, all_tokens;
    vector<int> line_number_map;
    string sentence;
    bool in_comment;
    int cur_pos, cur_line_number, len;

    bool _isBlank();
    bool _isCommentStart();
    bool _isCommentEnd();
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


#endif //AWESOMECC_LEXICAL_ANALYZER_HPP
