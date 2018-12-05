/**
 *
 * @file inter_code_generator.h
 * @brief 中间代码生成器类
 *
 * @author Keyi Li
 *
 */
#ifndef AWESOMECC_INTER_CODE_GENERATOR_H
#define AWESOMECC_INTER_CODE_GENERATOR_H

#include "../../lib/include/str_tools.h"
#include "../../lib/include/syntax_tree.h"

#include <map>
#include <string>
using std::map;
using std::string;


enum class INTER_CODE_OP_ENUM {
    ADD,
    SUB,
    DIV,
    MUL,
    JZ,  // 满足条件跳转
    JNZ, // 不满足条件跳转
    JLE, // arg1 < arg2 跳转
    JGE, // arg1 > arg2 跳转
    ASSIGN, // 赋值
    PRINT // 输出
};


enum class VARIABLE_INFO_ENUM {
    INT,
    DOUBLE,
    ARRAY
};


/**
 * @brief 变量信息类
 * @author Keyi Li
 */
class Info {
public:
    string name;
    VARIABLE_INFO_ENUM type;

    int int_value;
    double double_value;

    int place;

    Info();
    Info(string _name, VARIABLE_INFO_ENUM _type);
};


/**
 * @brief Operand parent类
 * @author Keyi Li
 */
class Operand {
};


class Variable : public Operand {
public:
    string name;
};


class Const : public Operand {
public:
    int int_value;
    double double_value;
};


class InterCodeIndex : public Operand {
public:
    int index;
};


/**
 * @brief 四元式类
 * @author Keyi Li
 */
class Quadruple {
public:
    INTER_CODE_OP_ENUM op;
    Operand arg1, arg2;
    Variable res;

    Quadruple(INTER_CODE_OP_ENUM _op, Operand _arg1, Operand _arg2, Variable _res);
};


/**
 * @brief 中间代码生成器类
 * @author Keyi Li
 */
class InterCodeGenerator {
private:
    SyntaxTree * tree; // 语法树
    int index; // 四元式个数统计
    int context_index; // 局部变量区分
    map<string, Info> table; // 变量表
    vector<Quadruple> inter_code; // 生成的四元式

    void _analyze(SyntaxTreeNode * cur);

    bool _lookUp(string identifier);
    void _emit(INTER_CODE_OP_ENUM op, Operand arg1, Operand arg2, Variable res);

    void _block(SyntaxTreeNode * cur);
    void _expression(SyntaxTreeNode * cur);
    void _statement(SyntaxTreeNode * cur);
    void _assignment(SyntaxTreeNode * cur);

public:
    InterCodeGenerator();
    void analyze(SyntaxTree * _tree);
};


#endif //AWESOMECC_INNER_CODE_GENERATOR_H
