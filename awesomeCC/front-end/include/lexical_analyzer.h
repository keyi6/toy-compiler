/**
 *
 * @file lexical_analyzer.h
 * @brief 词法分析器类
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
    vector<Token> tokens; // 当前句子的token列表
    vector<Token> all_tokens; // 所有token
    vector<int> line_number_map; // token index 和 行号的映射
    string sentence; // 目前分析的橘子
    int len; // 目前在分析的橘子的长度
    bool in_comment; // 现在是否在注释中的标识
    int cur_pos; // 目前的位置
    int cur_line_number; // 目前的行号

    bool _isBlank(); // 判断是不是空白字符
    bool _isCommentStart(); // 判断是不是评论开始
    bool _isCommentEnd(); // 判断是不是评论结束
    bool _isKeyword(string word); // 判断是不是关键字
    bool _isSeparator(char ch); // 判断是不是分隔符
    bool _isOperator(char ch);// 判断是不是预算符

    void _skipBlank();// 跳过空白和注释
    void _init(string _sentence); // 设置分析的橘子
    void _analyze(); // 进行词法分析

public:
    LexicalAnalyzer();
    vector<Token> getAllTokens(); // 获得所有all_token
    vector<int> getLineNumberMap();
    void analyze(vector<string> _sentences,bool verbose = true); // 词法分析
};


#endif //AWESOMECC_LEXICAL_ANALYZER_HPP
