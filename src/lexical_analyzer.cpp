/**
 *
 * @file lexical_analyzer.cpp
 * @brief 词法分析器，具体实现
 *
 * @author Keyi Li
 *
 */
#include "lexical_analyzer.hpp"

#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

/**
 * @brief LexicalAnalyzer类构造函数
 * @return LexicalAnalyzer object 返回一个词法分析类
 */
LexicalAnalyzer::LexicalAnalyzer() {
    _init("");
}

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
    char curChar = sentence[curPos];
    return (curChar == '\t' || curChar == '\n' || curChar == '\r' || curChar == ' ');
}


/**
 * @brief 自增curPos直到不为空
 */
void LexicalAnalyzer::_skipBlank() {
    while (curPos < len && _isBlank())
        curPos ++;
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
    curPos = 0;
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
    char curChar;

    while (curPos < len) {
        _skipBlank();

        curChar = sentence[curPos];

        // #include
        if (curChar == '#') {
            tokens.emplace_back(Token("#", 4, curPos ++));
            _skipBlank();
            if (sentence.substr(curPos, 7) == "include") {
                curPos += 7;
                tokens.emplace_back(Token("include", 0, curPos));
                _skipBlank();


                if (sentence[curPos] == '\"' || sentence[curPos] == '<') {
                    curChar = sentence[curPos ++];

                    tokens.emplace_back(Token(_char2string(curChar), 4, curPos));

                    char matchChar = curChar == '\"' ? '\"' : '>';

                    int libNameStart = curPos, libNameLen = 0;
                    while (curPos < len) {
                        if (sentence[curPos] == matchChar) {
                            tokens.emplace_back(Token(sentence.substr(libNameStart, libNameLen), 1, libNameStart));
                            tokens.emplace_back(Token(_char2string(matchChar), 4, curPos));
                            return;
                        }
                        else
                            libNameLen ++, curPos ++;
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
        else if (isalpha(curChar) || curChar == '_') {
            int tempLen = 0;
            while ((isalpha(sentence[curPos + tempLen]) || sentence[curPos + tempLen] == '_') && curPos + tempLen <= len)
                tempLen ++;

            string tempString = sentence.substr(curPos, tempLen);
            int temp_typeIndex = _isKeyword(tempString) ? 0 : 1;
            tokens.emplace_back(Token(tempString, temp_typeIndex, curPos));

            curPos += tempLen;
            continue;
        }
            // 数字常量
        else if (isdigit(curChar) || curChar == '.') {
            int tempLen = 0;
            bool hasDot = false;
            while ((isdigit(sentence[curPos + tempLen]) || sentence[curPos + tempLen] == '.') && curPos + tempLen <= len) {
                if (sentence[curPos + tempLen] == '.') {
                    if (not hasDot)
                        hasDot = true;
                    else {
                        errors.emplace_back(Error("Two many dots in a number."));
                        return;
                    }
                }

                tempLen++;
            }

            tokens.emplace_back(Token(sentence.substr(curPos, tempLen), 2, curPos));

            curPos += tempLen;
            continue;
        }
            // 分隔符 和 字符串常量
        else if (_isSeparator(curChar)) {
            string tempStr = sentence.substr(curPos, 1);
            tokens.emplace_back(Token(tempStr, 4, curPos));

            int tempLen = 0;
            if (curChar == '\"' || curChar == '\'') {
                curPos ++;

                while (sentence[curPos + tempLen] != curChar && curPos + tempLen < len)
                    tempLen ++;

                if (curPos + tempLen >= len || sentence[curPos + tempLen] != curChar) {
                    errors.emplace_back("Lack of " + _char2string(curChar) + ".");
                    return;
                }

                tokens.emplace_back(Token(sentence.substr(curPos, tempLen), 5, curPos + 1));
                curPos += tempLen;
                tokens.emplace_back(Token(tempStr, 4, curPos + 1));
            }

            curPos ++;
            continue;
        }
            // 运算符
        else if (_isOperator(curChar)) {
            // ++ -- << >>
            if ((curChar == '+' || curChar == '-' || curChar == '<' || curChar == '>') && curPos + 1 < len && sentence[curPos + 1] == curChar) {
                tokens.emplace_back(Token(sentence.substr(curPos, 2), 3, curPos));
                curPos += 2;
            }
                // <= >= !=
            else if ((curChar == '<' || curChar == '>' || curChar == '!') && curPos + 1 < len && sentence[curPos + 1] == '=') {
                tokens.emplace_back(Token(sentence.substr(curPos, 2), 3, curPos));
                curPos += 2;
            }
                // 一位的运算符
            else {
                tokens.emplace_back(Token(sentence.substr(curPos, 1), 3, curPos));
                curPos ++;
            }
        }

        curPos ++;
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

    _DEBUG_();

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
    curLineNumber = 0;
    allTokens.clear();

    int line = _sentences.size();
    for (auto _s: _sentences) {
        curLineNumber ++;
        _init(_s);
        _analyze();

        if (errors.empty()) {
            // TODO: handle right

        }
        else {
            // TODO: handle not right
            return false;
        }
    }

    return true;
}


/**
 * @brief 用于调试，输出tokens列表和errors列表
 */
void LexicalAnalyzer::_DEBUG_() {
    cout << "TOKENS:\n";

    for (auto t: tokens) {
        cout << "    [";
        cout << setw(10) << setfill(' ') << t.value;
        cout << "]    type:";
        cout << setw(15) << setfill(' ') << Token::TOKEN_TYPE[t.typeIndex];
        cout << endl;
    }

    cout << "ERRORS:\n";
    for (auto e: errors)
        cout << "    " << e.errorMsg << endl;
}


