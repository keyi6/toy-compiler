/**
 *
 * @file quadruple.h
 * @brief 四元式类
 *
 * @author Keyi Li
 *
 */

#ifndef AWESOMECC_QUADRUPLE_H
#define AWESOMECC_QUADRUPLE_H

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
    ADD,
    SUB,
    DIV,
    MUL,
    JZ,  // 满足条件跳转
    JNZ, // 不满足条件跳转
    JL, // arg1 < arg2 跳转
    JG, // arg1 > arg2 跳转
    ASSIGN, // 赋值
    PRINT // 输出
};


/**
 * @brief 四元式类
 * @author Keyi Li
 */
class Quadruple {
public:
    static vector<string> INTER_CODE_OP;
    static map<string, INTER_CODE_OP_ENUM> INTER_CODE_MAP;

    INTER_CODE_OP_ENUM op;
    string res, arg1, arg2;

    Quadruple(INTER_CODE_OP_ENUM _op, string arg1,
              string _arg2, string _res);

    friend ostream & operator << (ostream &out, Quadruple & q);
};


#endif //AWESOMECC_QUADRUPLE_H
