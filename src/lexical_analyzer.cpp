#include <utility>

//
// Created by Keyi Li on 2018/11/10.
//
#include "lexical_analyzer.hpp"

#include <cctype>
#include <string>
#include <vector>
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


bool LexicalAnalyzer::_isKeywords(string word) {
    for (string kw: Token::KEYWORDS)
        if (kw == word)
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
        else if (isalpha(curChar) || curChar == '_') {
            int tempLen = 0;
            while (isalpha(sentence[curPos + tempLen]) || sentence[curPos + tempLen] == '_')
                tempLen ++;

            string tempString = sentence.substr(curPos, tempLen);
            int temp_typeIndex = _isKeywords(tempString) ? 0 : 1;
            tokens.emplace_back(Token(tempString, temp_typeIndex));

            curPos = curPos + tempLen;
            continue;
        }
        else if (isdigit(curChar) || curChar == '.') {
            int tempLen = 0;
            bool hasDot = false;
            while (isalpha(sentence[curPos + tempLen]) || sentence[curPos + tempLen] == '.') {
                if (sentence[curPos + tempLen] == '.') {
                    if (not hasDot)
                        hasDot = true;
                    else
                        ;
                }
                tempLen++;
            }

        }

        curPos ++;
    }

}


void LexicalAnalyzer::analyze(string _sentence) {
    _init(move(_sentence));
    _analyze();

    _OUTPUT_TOKENS();
}


void LexicalAnalyzer::_OUTPUT_TOKENS() {
    for (auto t: tokens){
        cout << "[" << t.value << "] type:" << Token::TOKEN_TYPE[t.typeIndex] << endl;
    }
}