/**
 * @file token.cc
 * @brief Token类具体实现
 *
 * @author Keyi Li
 *
 */

#include "../include/token.h"

#include <iomanip>

using std::setw;
using std::setfill;
using std::endl;


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
        {"print", TOKEN_TYPE_ENUM::PRINT},
        {"class", TOKEN_TYPE_ENUM::CLASS},
        {"public", TOKEN_TYPE_ENUM::PUBLIC},
        {"private", TOKEN_TYPE_ENUM::PRIVATE},
        {"void", TOKEN_TYPE_ENUM::VOID},
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
        {"||", TOKEN_TYPE_ENUM::OR},
        {"&&", TOKEN_TYPE_ENUM::AND},
        {"==", TOKEN_TYPE_ENUM::EQUAL},
        {"*", TOKEN_TYPE_ENUM::MUL},
        {"%", TOKEN_TYPE_ENUM::MOD},
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
        "class", "public", "private",
        "if", "for", "while", "do", "else",
        "include", "return", "print"
};


vector<string> Token::OPERATORS = {"+",  "-",  "<",  ">", "!", "=", "*", "/", "%",
                                   "++", "--", "<<", ">>","||", "&&", "==",
                                   "<=", ">=", "!="};


vector<char> Token::SEPARATORS = {'(', ')', '{', '}', '[', ']', ',', '\'', ';' , '\'', '\"'};


/**
 * @brief Token狗仔函数
 * @author Keyi Li
 */
Token::Token(string _value, TOKEN_TYPE_ENUM _type, int _pos, int _line_number) {
    value = _value;
    type = _type;
    pos = _pos;
    line_number = _line_number;

    // 如果是分隔符、关键字和运算符的话，获取详细类别
    if (_type == TOKEN_TYPE_ENUM::SEPARATOR ||
        _type == TOKEN_TYPE_ENUM::KEYWORD ||
        _type == TOKEN_TYPE_ENUM::OPERATOR)
        type = Token::DETAIL_TOKEN_TYPE[_value];
}


/**
 * @brief 判断是不是表达式中的运算符
 * @author Keyi Li
 * @param t TOKEN_TYPE_ENUM
 * @return
 *      -<em>true</em> 是
 *      -<em>false</em> 否
 */
bool Token::isExpressionOperator(TOKEN_TYPE_ENUM t) {
    return TOKEN_TYPE_ENUM::AND <= t &&
           t <= TOKEN_TYPE_ENUM::RL_BRACKET;
}


/**
 * @brief 判断是不是单目运算符
 * @author Keyi Li
 * @param t TOKEN_TYPE_ENUM
 * @return
 *      -<em>true</em> 是
 *      -<em>false</em> 否
 */
bool Token::isUniOperator(TOKEN_TYPE_ENUM t) {
    return t == TOKEN_TYPE_ENUM::NOT||
           t == TOKEN_TYPE_ENUM::SELF_MINUS ||
           t == TOKEN_TYPE_ENUM::SELF_PLUS;
}


/**
 * @brief 判断是不是逻辑运算符
 * @author Keyi Li
 * @param t TOKEN_TYPE_ENUM
 * @return
 *      -<em>true</em> 是
 *      -<em>false</em> 否
 */
bool Token::isBoolOperator(TOKEN_TYPE_ENUM t) {
    return t == TOKEN_TYPE_ENUM::NOT ||
           (TOKEN_TYPE_ENUM::AND <= t &&
           t <= TOKEN_TYPE_ENUM::NOT_EQUAL);
}


/**
 * @brief 重载token输出流
 */
ostream & operator << (ostream & out, Token & t) {
    TOKEN_TYPE_ENUM detail_type = t.type;
    int general_type = int(detail_type);

    // 输出detail type太啰嗦 这里对应回去 输出general type就好
    if (TOKEN_TYPE_ENUM::INCLUDE <= detail_type &&
        detail_type <= TOKEN_TYPE_ENUM::RETURN)
        general_type = int(TOKEN_TYPE_ENUM::KEYWORD);
    else if (TOKEN_TYPE_ENUM::ASSIGN <= detail_type &&
        detail_type <= TOKEN_TYPE_ENUM::NOT)
        general_type = int(TOKEN_TYPE_ENUM::OPERATOR);
    else if (TOKEN_TYPE_ENUM::LL_BRACKET <= detail_type &&
        detail_type <= TOKEN_TYPE_ENUM::SHARP)
        general_type = int(TOKEN_TYPE_ENUM::SEPARATOR);

    out << setw(10) << setfill(' ') << t.value;
    out << setw(10) << setfill(' ') << "type: ";
    out << Token::TOKEN_TYPE[general_type];
    out << endl;
    return out;
}
