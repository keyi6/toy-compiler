/**
 *
 * @file lexical_analyzer.cpp
 * @brief 词法分析器，具体实现
 *
 * @author Keyi Li
 *
 */
#include "lexical_analyzer.h"

#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

/**
 * @brief LexicalAnalyzer类构造函数
 */
LexicalAnalyzer::LexicalAnalyzer() { }

/**
 * @brief 将字符转化为字符串
 * @param ch char, 输入字符
 * @return string string, 返回字符串
 */
string LexicalAnalyzer::_char2string(char ch) {
    stringstream ss;
    ss << ch;

    string ret = ss.str();
    ss.clear();

    return ret;
}

/**
 * @brief 判断curPos处是否为空字符
 * @return
 *      -<em>true</em> 是空字符
 *      -<em>false</em> 不是空字符
 */
bool LexicalAnalyzer::_isBlank() {
    char cur_char = sentence[cur_pos];
    return (cur_char == '\t' || cur_char == '\n' || cur_char == '\r' || cur_char == ' ');
}


/**
 * @brief 自增curPos直到不为空
 */
void LexicalAnalyzer::_skipBlank() {
    while (cur_pos < len && _isBlank())
        cur_pos ++;
}


/**
 * @brief 设置等待分析的句子，初始化
 * @param _sentence string, 等待分析的句子
 */
void LexicalAnalyzer::_init(string _sentence) {
    len = int(_sentence.length());
    sentence = _sentence;

    tokens.clear();
    errors.clear();
    cur_pos = 0;
}



/**
 * @brief 判断是否是关键词
 * @param word string, 等待分析的词
 * @return
 *      -<em>true</em> 是关键词
 *      -<em>false</em> 不是关键词
 */
bool LexicalAnalyzer::_isKeyword(string word) {
    for (string kw: Token::KEYWORDS)
        if (kw == word)
            return true;

    return false;
}

/**
 * @brief 判断是否是分隔符
 * @param ch char, 等待分析的字符
 * @return
 *      -<em>true</em> 是分隔符
 *      -<em>false</em> 不是分隔符
 */
bool LexicalAnalyzer::_isSeparator(char ch) {
    for (char sp: Token::SEPARATORS)
        if (sp == ch)
            return true;

    return false;
}


/**
 * @brief 判断是否是运算符
 * @param ch char, 等待分析的字符
 * @return
 *      -<em>true</em> 是运算符
 *      -<em>false</em> 不是运算符
 */
bool LexicalAnalyzer::_isOperator(char ch) {
    for (int i = 0; i < 10; i ++)
        if (ch == Token::OPERATORS[i][0])
            return true;

    return false;
}


/**
 * @brief 分析当前句子
 */
void LexicalAnalyzer::_analyze() {
    char cur_char;

    while (cur_pos < len) {
        _skipBlank();

        cur_char = sentence[cur_pos];

        // #include
        if (cur_char == '#') {
            tokens.emplace_back(Token("#", 4, cur_pos ++));
            _skipBlank();
            if (sentence.substr(cur_pos, 7) == "include") {
                cur_pos += 7;
                tokens.emplace_back(Token("include", 0, cur_pos));
                _skipBlank();


                if (sentence[cur_pos] == '\"' || sentence[cur_pos] == '<') {
                    cur_char = sentence[cur_pos ++];

                    tokens.emplace_back(Token(_char2string(cur_char), 4, cur_pos));

                    char match_char = cur_char == '\"' ? '\"' : '>';

                    int libNameStart = cur_pos, libNameLen = 0;
                    while (cur_pos < len) {
                        if (sentence[cur_pos] == match_char) {
                            tokens.emplace_back(Token(sentence.substr(libNameStart, libNameLen), 1, libNameStart));
                            tokens.emplace_back(Token(_char2string(match_char), 4, cur_pos));
                            return;
                        }
                        else
                            libNameLen ++, cur_pos ++;
                    }

                    errors.emplace_back(Error("include error"));
                }
            }
            else {
                errors.emplace_back(Error("include error"));

            }
            return;
        }
        // 关键字 和 标识符
        else if (isalpha(cur_char) || cur_char == '_') {
            int temp_len = 0;
            while ((isalpha(sentence[cur_pos + temp_len]) || sentence[cur_pos + temp_len] == '_') && cur_pos + temp_len <= len)
                temp_len ++;

            string temp_str = sentence.substr(cur_pos, temp_len);
            int temp_typeIndex = _isKeyword(temp_str) ? 0 : 1;
            tokens.emplace_back(Token(temp_str, temp_typeIndex, cur_pos));

            cur_pos += temp_len;
            continue;
        }

        // 数字常量
        else if (isdigit(cur_char) || cur_char == '.') {
            int temp_len = 0;
            bool hasDot = false;
            while ((isdigit(sentence[cur_pos + temp_len]) || sentence[cur_pos + temp_len] == '.') && cur_pos + temp_len <= len) {
                if (sentence[cur_pos + temp_len] == '.') {
                    if (not hasDot)
                        hasDot = true;
                    else {
                        errors.emplace_back(Error("Two many dots in a number."));
                        return;
                    }
                }

                temp_len++;
            }

            tokens.emplace_back(Token(sentence.substr(cur_pos, temp_len), 2, cur_pos));

            cur_pos += temp_len;
            continue;
        }
        // 分隔符 和 字符串常量
        else if (_isSeparator(cur_char)) {
            string temp_str = sentence.substr(cur_pos, 1);
            tokens.emplace_back(Token(temp_str, 4, cur_pos));

            int temp_len = 0;
            if (cur_char == '\"' || cur_char == '\'') {
                cur_pos ++;

                while (sentence[cur_pos + temp_len] != cur_char && cur_pos + temp_len < len)
                    temp_len ++;

                if (cur_pos + temp_len >= len || sentence[cur_pos + temp_len] != cur_char) {
                    errors.emplace_back("Lack of " + _char2string(cur_char) + ".");
                    return;
                }

                tokens.emplace_back(Token(sentence.substr(cur_pos, temp_len), 5, cur_pos + 1));
                cur_pos += temp_len;
                tokens.emplace_back(Token(temp_str, 4, cur_pos + 1));
            }

            cur_pos ++;
            continue;
        }
        // 运算符
        else if (_isOperator(cur_char)) {
            // ++ -- << >>
            if ((cur_char == '+' || cur_char == '-' || cur_char == '<' || cur_char == '>') && cur_pos + 1 < len && sentence[cur_pos + 1] == cur_char) {
                tokens.emplace_back(Token(sentence.substr(cur_pos, 2), 3, cur_pos));
                cur_pos += 2;
            }
                // <= >= !=
            else if ((cur_char == '<' || cur_char == '>' || cur_char == '!') && cur_pos + 1 < len && sentence[cur_pos + 1] == '=') {
                tokens.emplace_back(Token(sentence.substr(cur_pos, 2), 3, cur_pos));
                cur_pos += 2;
            }
                // 一位的运算符
            else {
                tokens.emplace_back(Token(sentence.substr(cur_pos, 1), 3, cur_pos));
                cur_pos ++;
            }
        }

        cur_pos ++;
    }

}


/**
 * @brief 分析一个句子，如果正确生成Token列表，如果错误生成错误列表
 * @param _sentence string, 等待分析的句子
 * @return
 *      -<em>true</em> 无错误
 *      -<em>false</em> 有错误
 */
bool LexicalAnalyzer::analyzeSentence(string _sentence) {
    _init(_sentence);
    _analyze();

    return ! errors.empty();
}

/**
 * @brief 分析一个程序（很多句子），如果正确生成Token列表，如果错误生成错误列表
 * @param _sentences vector<string>, 等待分析的程序
 * @return
 *      -<em>true</em> 无错误
 *      -<em>false</em> 有错误
 */
bool LexicalAnalyzer::analyze(vector<string> _sentences) {
    cur_line_number = 0;
    all_tokens.clear();

    int line = _sentences.size();
    for (auto _s: _sentences) {
        cur_line_number ++;
        _init(_s);
        _analyze();

        if (errors.empty()) {
            for (auto t: tokens)
                all_tokens.emplace_back(t);
        }
        else {
            cout << "Errors!" << endl;
            for (auto e: errors)
                cout << e;

            return false;
        }
    }

    cout << "Tokens\n";
    for (auto t: all_tokens)
        cout << t;

    return true;
}


/**
 * @brief 得到错误列表
 * @return vector<Error>
 */
vector<Error> LexicalAnalyzer::getAllError() {
    return errors;
}


/**
 * @brief 得到Token列表
 * @return vector<Token>
 */
vector<Token> LexicalAnalyzer::getAllTokens() {
    return all_tokens;
}
