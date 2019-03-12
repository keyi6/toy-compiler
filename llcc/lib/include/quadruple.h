/**
 * @file quadruple.h
 * @brief 四元式类
 */

#ifndef LLCC_QUADRUPLE_H
#define LLCC_QUADRUPLE_H

#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>

using std::map;
using std::cout;
using std::move;
using std::setw;
using std::endl;
using std::string;
using std::vector;
using std::setfill;
using std::ostream;
using std::ofstream;

enum class INTER_CODE_OP_ENUM {
    /* arithmetic */
    ADD,
    SUB,
    DIV,
    MUL,
    MOD,
    /* jump */
    J,     // 啥都不管 直接跳
    JE,    // arg1 == arg2 跳转
    JNE,   // arg1 == arg2 跳转
    JL,    // arg1 < arg2 跳转
    JG,    // arg1 > arg2 跳转
    /* other */
    MOV,   // 赋值
    PRINT, // 输出
    POP,
    PUSH
};


/**
 * @brief 四元式类
 */
class Quadruple {
public:
    static vector<string> INTER_CODE_OP;
    static map<string, INTER_CODE_OP_ENUM> INTER_CODE_MAP;
    static map<string, INTER_CODE_OP_ENUM> COUNTERPART_INTER_CODE_MAP;

    INTER_CODE_OP_ENUM op;
    string res, arg1, arg2;

    Quadruple(INTER_CODE_OP_ENUM _op, string arg1,
              string _arg2, string _res);

    friend ostream & operator << (ostream &out, Quadruple & q);
};


#endif //LLCC_QUADRUPLE_H
