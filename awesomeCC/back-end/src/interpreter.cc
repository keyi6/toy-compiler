/**
 *
 * @file virtual_machine.cc
 * @brief 解释执行程序
 *
 * @author Keyi Li
 *
 */

#include "../include/interpreter.h"
#define INCREMENT 100

Interpreter::Interpreter() = default;


void Interpreter::execute(vector<Quadruple> _code, bool verbose) {
    code = move(_code);
    index = 0;
    while (not activity.empty())
        activity.pop();

    // 初始大小 之后动态变换
    t_size = 100;
    t_stack.resize(t_size);
    v_size = 1000;
    v_stack.resize(v_size);

    int code_len = code.size();
    while (index < code_len)
        _execute(verbose);
}


void Interpreter::_execute(bool verbose) {
    int op = int(code[index].op);
    if (verbose)
        cout << "processing code #" << index << endl;

    switch (op) {
        case int(INTER_CODE_OP_ENUM::MOV):
            _assign();
            index ++;
            break;
        case int(INTER_CODE_OP_ENUM::ADD):
        case int(INTER_CODE_OP_ENUM::SUB):
        case int(INTER_CODE_OP_ENUM::MUL):
        case int(INTER_CODE_OP_ENUM::MOD):
        case int(INTER_CODE_OP_ENUM::DIV):
            _calc(op);
            index ++;
            break;
        case int(INTER_CODE_OP_ENUM::PRINT):
            _print();
            index ++;
            break;
        case int(INTER_CODE_OP_ENUM::J):
        case int(INTER_CODE_OP_ENUM::JE):
        case int(INTER_CODE_OP_ENUM::JNE):
        case int(INTER_CODE_OP_ENUM::JL):
        case int(INTER_CODE_OP_ENUM::JG):
            _jump();
            break;
        case int(INTER_CODE_OP_ENUM::POP):
            _pop();
            index ++;
            break;
        case int(INTER_CODE_OP_ENUM::PUSH):
            _push();
            index ++;
            break;
        default:
            break;
    }
}


void Interpreter::_print() {
    string value_str = code[index].arg1;
    if (value_str == "")
        cout << endl;
    else if (value_str[0] == '\"' || value_str[0] == '\'')
        cout << value_str.substr(1, value_str.size() - 2) << " ";
    else
        cout << _getValue(value_str) << " ";
}


void Interpreter::_calc(int op) {
    double a = _getValue(code[index].arg1);
    double b = _getValue(code[index].arg2);
    double value;

    switch (op) {
        case int(INTER_CODE_OP_ENUM::ADD):
            value = a + b;
            break;
        case int(INTER_CODE_OP_ENUM::SUB):
            value = a - b;
            break;
        case int(INTER_CODE_OP_ENUM::MUL):
            value = a * b;
            break;
        case int(INTER_CODE_OP_ENUM::DIV):
            value = a / b;
            break;
        case int(INTER_CODE_OP_ENUM::MOD):
            value = int(a) % int(b);
            break;
    }

    string res = code[index].res;
    int temp_index = _getAddress(res);
    // 读取res
    if (res[0] == 'v')
        v_stack[temp_index] = value;
    else
        t_stack[temp_index] = value;
}


void Interpreter::_assign() {
    string res = code[index].res;

    // 先读取右值
    double r_value = _getValue(code[index].arg1);

    int temp_index = _getAddress(res);

    // 再读取左值
    if (res[0] == 'v')
        v_stack[temp_index] = r_value;
    else
        t_stack[temp_index] = r_value;
}


void Interpreter::_jump() {
    INTER_CODE_OP_ENUM op = code[index].op;
    if (op == INTER_CODE_OP_ENUM::J) {
        index = int(_getValue(code[index].res));
        return;
    }

    double a = _getValue(code[index].arg1);
    double b = _getValue(code[index].arg2);

    if ((op == INTER_CODE_OP_ENUM::JE  && a == b) ||
        (op == INTER_CODE_OP_ENUM::JNE && a != b) ||
        (op == INTER_CODE_OP_ENUM::JG  && a > b) ||
        (op == INTER_CODE_OP_ENUM::JL  && a < b))
        index = int(_getValue(code[index].res));
    else
        index ++;
}


int Interpreter::_getAddress(string value_str) {
    if (value_str[0] == 'v') {
        int len = value_str.size();
        for (int i = 0; i < len; i ++) {
            if (value_str[i] == '[') {
                // 相对寻址
                int offset = _getValue(value_str.substr(i + 1, len - i - 2));
                int base = _getValue(value_str.substr(1, i - 1));
                return offset + base;
            }
        }

        int ret = string2int(value_str.substr(1));
        if (ret >= v_size) {
            v_size += INCREMENT;
            v_stack.resize(v_size);
        }

        return ret;
    }
    else {
        int ret = string2int(value_str.substr(1));
        if (ret >= t_size) {
            t_size += INCREMENT;
            t_stack.resize(t_size);
        }

        return ret;
    }
}


double Interpreter::_getValue(string value_str) {
    if (value_str[0] == 'p') {
        return index + string2int(value_str.substr(3));
    }
    // 寻址
    if (value_str[0] == 'v') {
        int len = value_str.size();
        for (int i = 0; i < len; i ++)
            if (value_str[i] == '[') {
                // 相对寻址 || 相对变址寻址
                int offset = _getValue(value_str.substr(i + 1, len - i - 2));
                int base = _getValue(value_str.substr(1, i - 1));
                return v_stack[offset + base];
            }

        // 立即数寻址
        int temp_index = string2int(value_str.substr(1));
        return v_stack[temp_index];
    }
    // 临时变量
    if (value_str[0] == 't') {
        int temp_index = string2int(value_str.substr(1));
        return t_stack[temp_index];
    }
    // 立即数
    else
        return string2double(value_str);
}


void Interpreter::_pop() {
    string res = code[index].res;
    double v = activity.top();
    activity.pop();
    cout << "debug [pop] " <<  v << endl;

    int temp_index = _getAddress(res);
    if (res[0] == 'v')
        v_stack[temp_index] = v;
    else
        t_stack[temp_index] = v;
}


void Interpreter::_push() {
    double v = _getValue(code[index].res);
    activity.push(v);
}
