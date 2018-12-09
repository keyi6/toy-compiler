/**
 *
 * @file lexical_analyzer.cc
 * @brief 词法分析器，具体实现
 *
 * @author Keyi Li
 *
 */

#include "../include/lexical_analyzer.h"

/**
 * @brief LexicalAnalyzer类构造函数
 */
LexicalAnalyzer::LexicalAnalyzer() {
    in_comment = false;
};


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
 * @brief 判断curPos处是是不是备注开始
 * @return
 *      -<em>true</em> 是备注开始
 *      -<em>false</em> 不是
 */
bool LexicalAnalyzer::_isCommentStart() {
    return (sentence[cur_pos] == '/' &&
        cur_pos + 1 < len &&
        sentence[cur_pos + 1] == '*');
}


/**
 * @brief 判断curPos处是是不是备注结束
 * @return
 *      -<em>true</em> 是备注结束
 *      -<em>false</em> 不是
 */
bool LexicalAnalyzer::_isCommentEnd() {
    return (sentence[cur_pos] == '*' &&
            cur_pos + 1 < len &&
            sentence[cur_pos + 1] == '/');
}


/**
 * @brief 自增curPos直到不为空
 */
void LexicalAnalyzer::_skipBlank() {
    while (cur_pos < len &&
          (_isBlank() || (in_comment && ! _isCommentEnd() ))) {
        cur_pos ++;
    }

    if (cur_pos < len && _isCommentEnd()) {
        in_comment = false;
        cur_pos += 2;

        if (cur_pos < len)
            _skipBlank();
    }
}


/**
 * @brief 设置等待分析的句子，初始化
 * @param _sentence string, 等待分析的句子
 */
void LexicalAnalyzer::_init(string _sentence) {
    len = int(_sentence.length());
    sentence = _sentence;

    tokens.clear();
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
    for (auto o: Token::OPERATORS)
        if (ch == o[0])
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

        if (_isCommentStart()) {
            in_comment = true;
            cur_pos += 2;
            continue;
        }
        // #include
        else if (cur_char == '#') {
            tokens.emplace_back(Token("#", TOKEN_TYPE_ENUM::SEPARATOR, cur_pos ++));
            _skipBlank();

            if (cur_pos + 7 < len && sentence.substr(cur_pos, 7) == "include") {
                // 读取include
                cur_pos += 7;
                tokens.emplace_back(Token("include", TOKEN_TYPE_ENUM::KEYWORD, cur_pos));
                _skipBlank();

                // 如果是 " 或者 《
                if (sentence[cur_pos] == '\"' || sentence[cur_pos] == '<') {
                    cur_char = sentence[cur_pos ++];

                    tokens.emplace_back(Token(char2string(cur_char), TOKEN_TYPE_ENUM::SEPARATOR, cur_pos));

                    char match_char = cur_char == '\"' ? '\"' : '>';

                    int libNameStart = cur_pos, libNameLen = 0;
                    while (cur_pos < len) {
                        if (sentence[cur_pos] == match_char) {
                            tokens.emplace_back(Token(sentence.substr(libNameStart, libNameLen), TOKEN_TYPE_ENUM::IDENTIFIER, libNameStart));
                            tokens.emplace_back(Token(char2string(match_char), TOKEN_TYPE_ENUM::SEPARATOR, cur_pos));
                            return;
                        }
                        else
                            libNameLen ++, cur_pos ++;
                    }

                    throw Error("in include, lack of " + char2string(match_char), cur_line_number);
                }
            }

            else
                throw Error("in include, unidentified symbol after #", cur_line_number);
        }
        // 关键字 和 标识符
        else if (isalpha(cur_char) || cur_char == '_') {
            int temp_len = 0;
            while (cur_pos + temp_len <= len && (isalpha(sentence[cur_pos + temp_len]) || sentence[cur_pos + temp_len] == '_') )
                temp_len ++;

            string temp_str = sentence.substr(cur_pos, temp_len);
            tokens.emplace_back(Token(temp_str,
                                      _isKeyword(temp_str) ? TOKEN_TYPE_ENUM::KEYWORD : TOKEN_TYPE_ENUM::IDENTIFIER,
                                      cur_pos));

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
                    else
                        throw Error("in digit constant, too many dots in one number", cur_line_number);
                }

                temp_len++;
            }

            tokens.emplace_back(Token(sentence.substr(cur_pos, temp_len), TOKEN_TYPE_ENUM::DIGIT_CONSTANT, cur_pos));

            cur_pos += temp_len;
            continue;
        }
        // 分隔符 和 字符串常量
        else if (_isSeparator(cur_char)) {
            string temp_str = sentence.substr(cur_pos, 1);
            tokens.emplace_back(Token(temp_str, TOKEN_TYPE_ENUM::SEPARATOR, cur_pos));

            int temp_len = 0;
            if (cur_char == '\"' || cur_char == '\'') {
                cur_pos ++;

                while (sentence[cur_pos + temp_len] != cur_char && cur_pos + temp_len < len)
                    temp_len ++;

                if (cur_pos + temp_len >= len || sentence[cur_pos + temp_len] != cur_char)
                    throw Error("in string constant, lack of " + char2string(cur_char), cur_line_number);


                tokens.emplace_back(Token(sentence.substr(cur_pos, temp_len), TOKEN_TYPE_ENUM::STRING_CONSTANT, cur_pos + 1));
                cur_pos += temp_len;
                tokens.emplace_back(Token(temp_str, TOKEN_TYPE_ENUM::SEPARATOR, cur_pos + 1));
            }

            cur_pos ++;
            continue;
        }
        // 运算符
        else if (_isOperator(cur_char)) {
            // ++ -- << >> && || ==
            if ((cur_char == '+' || cur_char == '-' || cur_char == '<' || cur_char == '>' ||
                 cur_char == '&' || cur_char == '|' || cur_char == '=')
            && cur_pos + 1 < len && sentence[cur_pos + 1] == cur_char) {
                tokens.emplace_back(Token(sentence.substr(cur_pos, 2), TOKEN_TYPE_ENUM::OPERATOR, cur_pos));
                cur_pos += 2;
            }
            // <= >= !=
            else if ((cur_char == '<' || cur_char == '>' || cur_char == '!') && cur_pos + 1 < len && sentence[cur_pos + 1] == '=') {
                tokens.emplace_back(Token(sentence.substr(cur_pos, 2), TOKEN_TYPE_ENUM::OPERATOR, cur_pos));
                cur_pos += 2;
            }
            // 一位的运算符
            else {
                tokens.emplace_back(Token(sentence.substr(cur_pos, 1), TOKEN_TYPE_ENUM::OPERATOR, cur_pos));
                cur_pos ++;
            }

            continue;
        }

        cur_pos ++;
    }
}


/**
 * @brief 分析一个程序（很多句子），如果正确生成Token列表，如果错误生成错误列表
 * @param _sentences vector string, 等待分析的程序
 * @param verbose bool, 是否就地输出tokens
 * @return
 *      -<em>true</em> 无错误
 *      -<em>false</em> 有错误
 */
bool LexicalAnalyzer::analyze(vector<string> _sentences, bool verbose) {
    cur_line_number = 0;
    in_comment = false;
    all_tokens.clear();

    try {
        for (auto _s: _sentences) {
            cur_line_number ++;

            _init(_s);
            _analyze();

            for (auto t: tokens) {
                all_tokens.emplace_back(t);
                line_number_map.emplace_back(cur_line_number);
            }
        }

        line_number_map.emplace_back(cur_line_number + 1);

        if (verbose) {
            cout << "Tokens\n";
            for (auto t: all_tokens)
                cout << t;
        }
    }
    catch (Error e) {
        cout << "Lexical analyze errors" << endl;
        cout << e;
        return false;
    }

    return true;
}


/**
 * @brief 得到Token列表
 * @return vector<Token>
 */
vector<Token> LexicalAnalyzer::getAllTokens() {
    return all_tokens;
}

/**
 * @brief 得到line_number_map列表
 * @return vector<int>
 */
vector<int> LexicalAnalyzer::getLineNumberMap() {
    return line_number_map;
}