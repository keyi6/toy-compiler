/**
 * @file lexical_analyzer.cc
 * @brief 词法分析器，具体实现
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
    return (cur_char == '\t' ||
            cur_char == '\n' ||
            cur_char == '\r' ||
            cur_char == ' ');
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
 * @brief 自增curPos直到不为空且不为注释中
 */
void LexicalAnalyzer::_skipBlank() {
    while (cur_pos < len &&
          (_isBlank() ||                      // 是空字符
          (in_comment && ! _isCommentEnd()))) // 或者在评论里
        cur_pos ++;

    if (cur_pos < len && _isCommentEnd()) {   // 判断是不是评论的结束
        in_comment = false;                   // 读取 `*/`
        cur_pos += 2;

        if (cur_pos < len)
            _skipBlank(); // 读完注释后继续跳过
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

        // 处理注释
        if (_isCommentStart()) {
            in_comment = true;
            cur_pos += 2;
            continue;
        }
        // 处理 #include
        else if (cur_char == '#') {
            tokens.emplace_back(Token("#", TOKEN_TYPE_ENUM::SEPARATOR,
                                      cur_pos ++, cur_line_number));
            _skipBlank();

            if (cur_pos + 7 < len && sentence.substr(cur_pos, 7) == "include") {
                // 读取include
                cur_pos += 7;
                tokens.emplace_back(Token("include", TOKEN_TYPE_ENUM::KEYWORD,
                                          cur_pos, cur_line_number));
                _skipBlank();

                // 如果是 " 或者 <
                if (sentence[cur_pos] == '\"' || sentence[cur_pos] == '<') {
                    // 读取这个字符，用于检查配对
                    cur_char = sentence[cur_pos ++];

                    tokens.emplace_back(Token(char2string(cur_char), TOKEN_TYPE_ENUM::SEPARATOR,
                                              cur_pos, cur_line_number));

                    char match_char = cur_char == '\"' ? '\"' : '>';
                    int lib_name_start = cur_pos, lib_name_len = 0;
                    while (cur_pos < len) {
                        if (sentence[cur_pos] == match_char) {
                            tokens.emplace_back(Token(sentence.substr(lib_name_start, lib_name_len),
                                                      TOKEN_TYPE_ENUM::IDENTIFIER,
                                                      lib_name_start, cur_line_number));

                            tokens.emplace_back(Token(char2string(match_char),
                                                      TOKEN_TYPE_ENUM::SEPARATOR,
                                                      cur_pos, cur_line_number));

                            _skipBlank();
                            if (cur_pos < len)
                                throw Error("in include, unidentified token after " + char2string(match_char),
                                            cur_line_number, cur_pos);
                            return;
                        }
                        else
                            lib_name_len ++, cur_pos ++;
                    }
                    // 找不到match_char 的时候执行
                    throw Error("in include, lack of " + char2string(match_char),
                                cur_line_number, cur_pos);
                }
                else
                    throw Error("in include, unidentified symbol after `include`",
                                cur_line_number, cur_pos);
            }
            else
                throw Error("in include, unidentified symbol after #",
                            cur_line_number, cur_pos);
        }
        // 关键字 和 标识符
        else if (isalpha(cur_char) || cur_char == '_') {
            // 读取这个词
            int temp_len = 0;
            while (cur_pos + temp_len <= len &&
                  (isalpha(sentence[cur_pos + temp_len]) || sentence[cur_pos + temp_len] == '_'))
                temp_len ++;

            string temp_str = sentence.substr(cur_pos, temp_len);
            tokens.emplace_back(Token(temp_str,
                                      _isKeyword(temp_str) ? TOKEN_TYPE_ENUM::KEYWORD : TOKEN_TYPE_ENUM::IDENTIFIER,
                                      cur_pos, cur_line_number));

            cur_pos += temp_len;
            continue;
        }
        // 数字常量
        else if (isdigit(cur_char) || cur_char == '.') {
            int temp_len = 0;
            bool hasDot = false;
            while (cur_pos + temp_len <= len &&
                  (isdigit(sentence[cur_pos + temp_len]) ||
                   sentence[cur_pos + temp_len] == '.')) {

                if (sentence[cur_pos + temp_len] == '.') {
                    if (not hasDot)
                        hasDot = true;
                    else
                        throw Error("in digit constant, too many dots in one number",
                                    cur_line_number, cur_pos);
                }

                temp_len ++;
            }

            tokens.emplace_back(Token(sentence.substr(cur_pos, temp_len),
                                      TOKEN_TYPE_ENUM::DIGIT_CONSTANT,
                                      cur_pos, cur_line_number));

            cur_pos += temp_len;
            continue;
        }
        // 分隔符 和 字符串常量
        else if (_isSeparator(cur_char)) {
            string temp_str = sentence.substr(cur_pos, 1);
            tokens.emplace_back(Token(temp_str, TOKEN_TYPE_ENUM::SEPARATOR,
                                      cur_pos, cur_line_number));

            // 如果是 `'`或者`"` 需要考虑一下匹配
            int temp_len = 0;
            if (cur_char == '\"' || cur_char == '\'') {
                cur_pos ++;

                while (cur_pos + temp_len < len &&
                       sentence[cur_pos + temp_len] != cur_char)
                    temp_len ++;

                // 匹配不上
                if (cur_pos + temp_len >= len || sentence[cur_pos + temp_len] != cur_char)
                    throw Error("in string constant, lack of " + char2string(cur_char),
                                cur_line_number, cur_pos);

                tokens.emplace_back(Token(sentence.substr(cur_pos, temp_len),
                                          TOKEN_TYPE_ENUM::STRING_CONSTANT,
                                          cur_pos + 1, cur_line_number));

                cur_pos += temp_len;
                tokens.emplace_back(Token(temp_str, TOKEN_TYPE_ENUM::SEPARATOR,
                                          cur_pos + 1, cur_line_number));
            }

            cur_pos ++;
            continue;
        }
        // 运算符
        else if (_isOperator(cur_char)) {
            // ++ -- << >> && || ==
            if ((cur_char == '+' || cur_char == '-' || cur_char == '<' || cur_char == '>' ||
                 cur_char == '&' || cur_char == '|' || cur_char == '=') &&
                cur_pos + 1 < len &&
                sentence[cur_pos + 1] == cur_char) {
                tokens.emplace_back(Token(sentence.substr(cur_pos, 2), TOKEN_TYPE_ENUM::OPERATOR,
                                          cur_pos, cur_line_number));
                cur_pos += 2;
            }
            // <= >= !=
            else if ((cur_char == '<' || cur_char == '>' || cur_char == '!') &&
                     cur_pos + 1 < len &&
                     sentence[cur_pos + 1] == '=') {
                tokens.emplace_back(Token(sentence.substr(cur_pos, 2), TOKEN_TYPE_ENUM::OPERATOR,
                                          cur_pos, cur_line_number));
                cur_pos += 2;
            }
            // 一位的运算符
            else {
                tokens.emplace_back(Token(sentence.substr(cur_pos, 1), TOKEN_TYPE_ENUM::OPERATOR,
                                    cur_pos, cur_line_number));
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
 */
void LexicalAnalyzer::analyze(vector<string> _sentences, bool verbose) {
    cur_line_number = 0;
    in_comment = false;
    all_tokens.clear();

    try {
        for (auto _s: _sentences) {
            cur_line_number ++;

            _init(_s);
            _analyze();

            for (auto t: tokens)
                all_tokens.emplace_back(t);
        }

        if (verbose) {
            cout << "Tokens\n";
            for (auto t: all_tokens)
                cout << t;
        }
    }
    catch (Error & e) {
        cout << "Lexical analyze errors" << endl;
        cout << e;

        exit(0);
    }
}


/**
 * @brief 得到Token列表
 * @return vector<Token>
 */
vector<Token> LexicalAnalyzer::getAllTokens() {
    return all_tokens;
}
