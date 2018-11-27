/**
 * @file token.cc
 * @brief Token类具体实现
 *
 * @author Keyi Li
 *
 */
#include "token.h"



vector<string> Token::TOKEN_TYPE = {
        "keyword",
        "identifier",
        "digit constant",
        "operator",
        "separator",
        "string constant"
};


map<string, TOKEN_TYPE_ENUM> Token::DETAIL_TOKEN_TYPE = {
        {"include", TOKEN_TYPE_ENUM::INCLUDE},
        {"int", TOKEN_TYPE_ENUM::INT},
        {"float", TOKEN_TYPE_ENUM::FLOAT},
        {"char", TOKEN_TYPE_ENUM::CHAR},
        {"double", TOKEN_TYPE_ENUM::DOUBLE},
        {"for", TOKEN_TYPE_ENUM::FOR},
        {"if", TOKEN_TYPE_ENUM::IF},
        {"else", TOKEN_TYPE_ENUM::ELSE},
        {"while", TOKEN_TYPE_ENUM::WHILE},
        {"do", TOKEN_TYPE_ENUM::DO},
        {"return", TOKEN_TYPE_ENUM::RETURN},
        {"+", TOKEN_TYPE_ENUM::PLUS},
        {"-", TOKEN_TYPE_ENUM::MINUS},
        {"<", TOKEN_TYPE_ENUM::LT},
        {">", TOKEN_TYPE_ENUM::GT},
        {"!", TOKEN_TYPE_ENUM::NOT},
        {"=", TOKEN_TYPE_ENUM::ASSIGN},
        {"|", TOKEN_TYPE_ENUM::OR},
        {"&", TOKEN_TYPE_ENUM::ADDRESS},
        {"*", TOKEN_TYPE_ENUM::MUL},
        {"/", TOKEN_TYPE_ENUM::DIV},
        {"++", TOKEN_TYPE_ENUM::SELF_PLUS},
        {"--", TOKEN_TYPE_ENUM::SELF_MINUS},
        {">>", TOKEN_TYPE_ENUM::RIGHT_SHIFT},
        {"<<", TOKEN_TYPE_ENUM::LEFT_SHIFT},
        {">=", TOKEN_TYPE_ENUM::GET},
        {"<=", TOKEN_TYPE_ENUM::LET},
        {"!=", TOKEN_TYPE_ENUM::NOT_EQUAL},
        {"(", TOKEN_TYPE_ENUM::LL_BRACKET},
        {")", TOKEN_TYPE_ENUM::RL_BRACKET},
        {"{", TOKEN_TYPE_ENUM::LB_BRACKET},
        {"}", TOKEN_TYPE_ENUM::RB_BRACKET},
        {"[", TOKEN_TYPE_ENUM::LM_BRACKET},
        {"]", TOKEN_TYPE_ENUM::RM_BRACKET},
        {",", TOKEN_TYPE_ENUM::COMMA},
        {"\"", TOKEN_TYPE_ENUM::DOUBLE_QUOTE},
        {"\'", TOKEN_TYPE_ENUM::SINGLE_QUOTE},
        {";", TOKEN_TYPE_ENUM::SEMICOLON},
        {"#", TOKEN_TYPE_ENUM::SHARP},
};


vector<string> Token::KEYWORDS = {
        "int", "float", "double", "char", "void",
        "if", "for", "while", "do", "else",
        "include", "return"
};


vector<string> Token::OPERATORS = {"+",  "-",  "<",  ">", "!", "=", "|", "&", "*", "/",  // 0 - 9
                                   "++", "--", "<<", ">>",                               // 10 - 13
                                   "<=", ">=", "!="};                        // 14 - 16

vector<char> Token::SEPARATORS = {'(', ')', '{', '}', '[', ']', ',', '\'', ';' };


Token::Token(string _value, TOKEN_TYPE_ENUM _type, int _pos) {
    value = _value;
    type = _type;
    pos = _pos;

    if (_type == TOKEN_TYPE_ENUM::SEPARATOR || _type == TOKEN_TYPE_ENUM::KEYWORD || _type == TOKEN_TYPE_ENUM::OPERATOR)
        type = Token::DETAIL_TOKEN_TYPE[_value];
}


ostream & operator << (ostream & out, Token & t) {
    int _ = int(t.type);
    if (int(TOKEN_TYPE_ENUM::SEPARATOR) <= _ && _ <= int(TOKEN_TYPE_ENUM::RETURN))
        _ = int(TOKEN_TYPE_ENUM::KEYWORD);
    else if (int(TOKEN_TYPE_ENUM::PLUS) <= _ && _ <= int(TOKEN_TYPE_ENUM::NOT_EQUAL))
        _ = int(TOKEN_TYPE_ENUM::OPERATOR);
    else if (int(TOKEN_TYPE_ENUM::LL_BRACKET) <= _ && _ <= int(TOKEN_TYPE_ENUM::SHARP))
        _ = int(TOKEN_TYPE_ENUM::SEPARATOR);

    out << setw(10) << setfill(' ') << t.value;
    out << setw(10) << setfill(' ') << "type: " << Token::TOKEN_TYPE[_] << endl;
    return out;
}
