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
        "ADD", "SUB", "DIV", "MUL", "MOD",
        "J", "JE", "JNE", "JL", "JG",
        "MOV", "PRINT", "POP", "PUSH"
};



map<string, INTER_CODE_OP_ENUM> Quadruple::INTER_CODE_MAP = {
        {"+", INTER_CODE_OP_ENUM::ADD},
        {"-", INTER_CODE_OP_ENUM::SUB},
        {"*", INTER_CODE_OP_ENUM::MUL},
        {"%", INTER_CODE_OP_ENUM::MOD},
        {"/", INTER_CODE_OP_ENUM::DIV},

        {"==", INTER_CODE_OP_ENUM::JE},
        {"!=", INTER_CODE_OP_ENUM::JNE},
        {">", INTER_CODE_OP_ENUM::JG},
        {"<", INTER_CODE_OP_ENUM::JL},

        {"ADD", INTER_CODE_OP_ENUM::ADD},
        {"SUB", INTER_CODE_OP_ENUM::SUB},
        {"MUL", INTER_CODE_OP_ENUM::MUL},
        {"MOD", INTER_CODE_OP_ENUM::MOD},
        {"DIV", INTER_CODE_OP_ENUM::DIV},

        {"J", INTER_CODE_OP_ENUM::J},
        {"JE", INTER_CODE_OP_ENUM::JE},
        {"JNE", INTER_CODE_OP_ENUM::JNE},
        {"JG", INTER_CODE_OP_ENUM::JG},
        {"JL", INTER_CODE_OP_ENUM::JL},

        {"MOV", INTER_CODE_OP_ENUM::MOV},
        {"PRINT", INTER_CODE_OP_ENUM::PRINT},
        {"POP", INTER_CODE_OP_ENUM::POP},
        {"PUSH", INTER_CODE_OP_ENUM::PUSH},
};


map<string, INTER_CODE_OP_ENUM> Quadruple::COUNTERPART_INTER_CODE_MAP = {
        {"==", INTER_CODE_OP_ENUM::JNE},
        {"!=", INTER_CODE_OP_ENUM::JE},
        {">", INTER_CODE_OP_ENUM::JL},
        {"<", INTER_CODE_OP_ENUM::JG},
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
    out << setw(6) << setfill (' ') << q.arg1 << ", ";
    out << setw(6) << setfill (' ') << q.arg2 << ", ";
    out << setw(6) << setfill (' ') << q.res;
    out << ")" << endl;

    return out;
}