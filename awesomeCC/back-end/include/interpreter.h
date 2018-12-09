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

#include <vector>

using std::vector;


class Interpreter {
private:
    int index; // 我的pc指针
    vector<Quadruple> code;
    vector<double> t_stack;
    vector<double> v_stack;

    double _getValue(string value_str);
    int _getAddress(string value_str);

    void _calc(int op);
    void _execute();
    void _print();
    void _assign();
    void _jump();

public:
    Interpreter();
    void execute(vector<Quadruple> _code);
};


#endif //AWESOMECC_VIRTUAL_MACHINE_H
