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

#define V(x) (x).begin(),(x).end()

#include "../../lib/include/error.h"
#include "../../lib/include/str_tools.h"
#include "../../lib/include/quadruple.h"
#include "../../lib/include/syntax_tree.h"

#include <map>
#include <stack>
#include <regex>
#include <string>
#include <iomanip>
#include <fstream>
#include <iostream>

using std::map;
using std::cout;
using std::move;
using std::setw;
using std::endl;
using std::stack;
using std::regex;
using std::string;
using std::setfill;
using std::ostream;
using std::ofstream;
using std::regex_replace;


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
    int place;

    Info();
    Info(VARIABLE_INFO_ENUM _type, int _place);
};


/**
 * @brief 中间代码生成器类
 * @author Keyi Li
 */
class InterCodeGenerator {
private:
    SyntaxTree * tree; // 语法树
    int temp_var_index; // 临时变量栈顶
    int var_index; // 用户的变量栈顶
    int context_index; // 局部变量区分
    map<string, Info> table; // 变量表
    vector<Quadruple> inter_code; // 生成的四元式

    void _analyze(SyntaxTreeNode * cur);

    string _lookUp(string name);
    string _lookUp(SyntaxTreeNode * arr_pointer);
    void _emit(INTER_CODE_OP_ENUM op, string arg1,
               string arg2, string res);

    string _locateArrayItem(string arr_name, string arr_i);
    string _expression(SyntaxTreeNode * cur);
    void _block(SyntaxTreeNode * cur);
    void _print(SyntaxTreeNode * cur);
    void _statement(SyntaxTreeNode * cur);
    void _assignment(SyntaxTreeNode * cur);
    void _if(SyntaxTreeNode * cur);
    void _while(SyntaxTreeNode * cur);

    void _backpatch(vector<int> v, int dest_index);


public:
    InterCodeGenerator();
    void analyze(SyntaxTree * _tree, bool verbose = false);
    void saveToFile(string path);
};


#endif //AWESOMECC_INNER_CODE_GENERATOR_H
