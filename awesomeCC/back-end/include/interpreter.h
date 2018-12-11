/**
 *
 * @file virtual_machine.cc
 * @brief 解释执行程序
 *
 * @author Keyi Li
 *
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
    int index; // 我的pc指针
    vector<Quadruple> code;
    vector<double> t_stack;
    vector<double> v_stack;
    stack<double> activity;

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
