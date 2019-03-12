/**
 * @file virtual_machine.cc
 * @brief 解释执行程序
 */
#ifndef AWESOMECC_VIRTUAL_MACHINE_H
#define AWESOMECC_VIRTUAL_MACHINE_H

#include "../../lib/include/str_tools.h"
#include "../../lib/include/quadruple.h"

#include <stack>
#include <string>
#include <vector>

using std::stack;
using std::string;
using std::vector;


class Interpreter {
private:
    int index;              // 我的pc指针
    int v_size;             // 变量表
    int t_size;             // 零食变量表
    vector<Quadruple> code; // 代码
    vector<double> t_stack; // 零食变量栈
    vector<double> v_stack; // 变量栈
    stack<double> activity; // 活动栈

    double _getValue(string value_str);
    int _getAddress(string value_str);

    void _calc(int op);
    void _execute(bool verbose = false);
    void _print();
    void _assign();
    void _jump();
    void _pop();
    void _push();

public:
    Interpreter();
    void execute(vector<Quadruple> _code, bool verbose = false);
};


#endif //AWESOMECC_VIRTUAL_MACHINE_H
