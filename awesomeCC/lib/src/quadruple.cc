/**
 *
 * @file quadruple.cc
 * @brief 四元式类具体实现
 *
 * @author Keyi Li
 *
 */

#include "../include/quadruple.h"


vector<string> Quadruple::INTER_CODE_OP = {
        "ADD", "SUB", "DIV", "MUL",
        "JZ", "JNZ", "JLE", "JGE",
        "MOV", "PRINT"
};

map<string, INTER_CODE_OP_ENUM> Quadruple::INTER_CODE_MAP = {
        {"+", INTER_CODE_OP_ENUM::ADD},
        {"-", INTER_CODE_OP_ENUM::SUB},
        {"*", INTER_CODE_OP_ENUM::MUL},
        {"/", INTER_CODE_OP_ENUM::DIV}
};


/**
 * @brief 四元式构造函数
 * @author Keyi Li
 */
Quadruple::Quadruple(INTER_CODE_OP_ENUM _op, string _arg1, string _arg2, string _res) {
    op = _op;
    arg1 = _arg1;
    arg2 = _arg2;
    res = _res;
}


/**
 * @brief 四元式输出流
 * @author Keyi Li
 */
ostream & operator << (ostream & out, Quadruple & q) {
    out << "(";
    out << setw(6) << setfill (' ') << Quadruple::INTER_CODE_OP[int(q.op)] << ", ";
    out << setw(5) << setfill (' ') << q.arg1 << ", ";
    out << setw(5) << setfill (' ') << q.arg2 << ", ";
    out << setw(5) << setfill (' ') << q.res;
    out << ")" << endl;

    return out;
}