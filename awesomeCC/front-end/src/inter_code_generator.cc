/**
 *
 * @file inter_code_generator.h
 * @brief 中间代码生成器类具体实现
 *
 * @author Keyi Li
 *
 */

#include "../include/inter_code_generator.h"


/**
 * @brief Info构造函数
 */
Info::Info() = default;


/**
 * @brief Info构造函数
 * @param _name 变量名字
 * @param _type 种类
 */
Info::Info(string _name, VARIABLE_INFO_ENUM _type) {
    name = _name;
    type = _type;
}


/**
 * @brief 四元式构造函数
 */
Quadruple::Quadruple(INTER_CODE_OP_ENUM _op, Operand _arg1, Operand _arg2, Variable _res) : op(_op), arg1(_arg1), arg2(_arg2), res(_res) {
}


/**
 * @brief 中间代码生成器构造函数
 * @author Keyi Li
 */
InterCodeGenerator::InterCodeGenerator() = default;


/**
 * @brief 中间代码生成
 * @param _tree SyntaxTree *
 * @author Keyi Li
 */
void InterCodeGenerator::analyze(SyntaxTree * _tree) {
    tree = _tree;
    inter_code.clear();
    index = 0;
    context_index = 0;

    tree -> display(true);

    _analyze(tree -> root);
}


/**
 * @brief 中间代码生成
 * @param _tree SyntaxTree *
 * @author Keyi Li
 */
void InterCodeGenerator::_analyze(SyntaxTreeNode * cur) {
    if (cur -> value == "FunctionStatement") {
        // TODO 函数声明
        SyntaxTreeNode * type_tree, * name_tree, * param_tree, * block_tree;
        SyntaxTreeNode * cs = cur -> first_son;
        while (cs) {
            if (cs -> value == "FunctionName")
                name_tree = cs -> first_son;
            else if (cs -> value == "Type")
                type_tree = cs -> first_son;
            else if (cs -> value == "Block")
                block_tree = cs;
            else if (cs -> value == "ParameterList")
                param_tree = cs;

            cs = cs -> right;
        }

        // main 函数直接执行
        if (name_tree -> value == "main") {
            _block(block_tree);
        }
    }
    // TODO 其他
}


/**
 * @brief 翻译block
 * @author Keyi Li
 */
void InterCodeGenerator::_block(SyntaxTreeNode * cur) {
    context_index ++;

    SyntaxTreeNode * cs = cur -> first_son;
    while (cs) {
        if (cs -> value == "Statement")
            _statement(cs);
        else if (cs -> value == "Assignment")
            _assignment(cs);

        cs = cs -> right;
    }
}


/**
 * @brief 翻译赋值语句
 * @author Keyi Li
 */
void InterCodeGenerator::_assignment(SyntaxTreeNode *cur) {
    SyntaxTreeNode * cs = cur -> first_son;

    _expression(cs -> right);
    //_emit(INTER_CODE_OP_ENUM::ASSIGN, , , 0);
}


/**
 * @brief 翻译表达式
 * @author Keyi Li
 */
void InterCodeGenerator::_expression(SyntaxTreeNode * cur) {
}


/**
 * @brief 翻译变量声明语句
 * @author Keyi Li
 */
void InterCodeGenerator::_statement(SyntaxTreeNode * cur) {
    SyntaxTreeNode * cs = cur -> first_son;

    while (cs) {
        if (cs -> type == "double" || cs -> type == "float") {
            Info info(cs -> value, VARIABLE_INFO_ENUM::DOUBLE);
            table[cs -> value] = info;
        }
        else if (cs -> type == "int") {
            Info info(cs -> value, VARIABLE_INFO_ENUM::INT);
            table[cs -> value] = info;
        }
        else {
            // TODO;
        }

        cs = cs -> right;
    }
}


/**
 * @brief 寻找标识符
 * @param identifier 标识符
 * @author Keyi Li
 */
bool InterCodeGenerator::_lookUp(string name) {
    return table.find(name) != table.end();
}


/**
 * @brief 生成一个四元式
 * @param op 操作符
 * @param arg1 参数1
 * @param arg2 参数2
 * @param res 结果
 * @author Keyi Li
 */
void InterCodeGenerator::_emit(INTER_CODE_OP_ENUM op, Operand arg1, Operand arg2, Variable res) {
    inter_code.emplace_back(Quadruple(op, arg1, arg2, res));
}
