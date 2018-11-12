//
// Created by Keyi Li on 2018/11/10.
//
#include "lexical_analyzer.hpp"

#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;


LexicalAnalyzer::LexicalAnalyzer() {
    _init("");
}


bool LexicalAnalyzer::_isBlank() {
    char curChar = sentence[curPos];
    return (curChar == '\t' || curChar == '\n' || curChar == '\r' || curChar == ' ');
}


void LexicalAnalyzer::_skipBlank() {
    while (curPos < len && _isBlank())
        curPos ++;
}


void LexicalAnalyzer::_init(string _sentence) {
    len = int(_sentence.length());
    sentence = _sentence;

    tokens.clear();
    curPos = 0;
}


bool LexicalAnalyzer::_isKeyword(string word) {
    for (string kw: Token::KEYWORDS)
        if (kw == word)
            return true;

    return false;
}


bool LexicalAnalyzer::_isSeparator(char ch) {
    for (char sp: Token::SEPARATORS)
        if (sp == ch)
            return true;

    return false;
}


bool LexicalAnalyzer::_isOperator(char ch) {
    for (int i = 0; i < 10; i ++)
        if (ch == Token::OPERATORS[i][0])
            return true;

    return false;
}


void LexicalAnalyzer::_analyze() {
    char curChar;

    while (curPos < len) {
        _skipBlank();

        curChar = sentence[curPos];

        if (curChar == '#') {
            // TODO: handle #include
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
                        errors.emplace_back(Error(curLineNumber, "Two many dots in a number."));
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
                    stringstream ss;
                    ss << "Lack of " << curChar << ".";
                    errors.emplace_back(curLineNumber, ss.str());
                    ss.clear();
                    return;
                }

                tokens.emplace_back(Token(sentence.substr(curPos, tempLen), 5, curPos + 1));
                curPos += tempLen;
                tokens.emplace_back(Token(tempStr, 4, curPos + 1));
            }

            curPos ++;
            continue;
        }
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
            else {
                tokens.emplace_back(Token(sentence.substr(curPos, 1), 3, curPos));
                curPos ++;
            }
        }

        curPos ++;
    }

}


void LexicalAnalyzer::analyze(string _sentence, int _curLineNumber) {
    curLineNumber = _curLineNumber;

    _init(move(_sentence));
    _analyze();

    _DEBUG_();
}


void LexicalAnalyzer::_DEBUG_() {
    for (auto t: tokens)
        cout << "[" << t.value << "] type:" << Token::TOKEN_TYPE[t.typeIndex] << endl;

    for (auto e: errors)
        e.output();
}
