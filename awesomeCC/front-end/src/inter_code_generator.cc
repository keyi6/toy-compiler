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
 * @author Keyi Li
 */
Info::Info(VARIABLE_INFO_ENUM _type, int _place) {
    name = "v" + int2string(_place);
    place = _place;
    type = _type;
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
void InterCodeGenerator::analyze(SyntaxTree * _tree, bool verbose) {
    inter_code.clear();
    temp_var_index = 0;
    context_index = 0;
    while (not zipper_stack.empty())
        zipper_stack.pop();

    tree = _tree;

    if (verbose)
        tree -> display(true);

    try {
        _analyze(tree -> root -> first_son);
    }
    catch (Error & e) {
        cout << "Semantic analyze errors" << endl;
        cout << e;
        exit(0);
    }

    if (verbose) {
        cout << "Generated " << inter_code.size() << " inter codes" << endl;
        for (int i = 0; i < inter_code.size(); i ++) {
            cout << "#" << setw(2) << setfill(' ') << i;
            cout << inter_code[i];
        }
    }
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
        else if (cs -> value == "Print")
            _print(cs);
        else if (cs -> value == "Control-If")
            _if(cs);

        // TODO 其他

        cs = cs -> right;
    }
}




/**
 * @brief 翻译Print
 * @author Keyi Li
 */
void InterCodeGenerator::_if(SyntaxTreeNode * cur) {
    SyntaxTreeNode * cs = cur -> first_son;
    while (cs) {
        if (cs -> value == "Control-Condition") {
            // 读取条件语句
            _expression(cs -> first_son);

            // 读取紧随其后的执行语句
            cs = cs -> right;
            _block(cs);

            // 回填一下
            zipperFill(inter_code.size() - 1);

            // 入回填栈
            zipper_stack.push(make_pair(inter_code.size(), 1));
            _emit(INTER_CODE_OP_ENUM::J, "", "", "");
        }
        else {
            // 回填一下
            zipperFill(inter_code.size());

            _block(cs);
        }

        cs = cs -> right;
    }
}


/**
 * @brief 翻译Print
 * @author Keyi Li
 */
void InterCodeGenerator::_print(SyntaxTreeNode * cur) {
    string print_place = _expression(cur -> first_son);
    _emit(INTER_CODE_OP_ENUM::PRINT, print_place, "", "");
}


/**
 * @brief 翻译赋值语句
 * @author Keyi Li
 */
void InterCodeGenerator::_assignment(SyntaxTreeNode * cur) {
    SyntaxTreeNode * cs = cur -> first_son;

    string r_value_place = _expression(cs -> right);

    string store_place;
    if (cs -> value == "Expression-ArrayItem")
        store_place = _lookUp(cs);
    else
        store_place = _lookUp(cur -> first_son -> value);

    _emit(INTER_CODE_OP_ENUM::MOV, r_value_place, "", store_place);
}


/**
 * @brief 翻译表达式
 * @author Keyi Li
 * @param cur 一个Expression-*节点执政
 * @return place, string
 */
string InterCodeGenerator::_expression(SyntaxTreeNode * cur, string previous) {
    // 双目运算符
    if (cur -> value == "Expression-DoubleOp" || cur -> value == "Expression-Bool-DoubleOp") {
        SyntaxTreeNode * a = cur -> first_son;
        SyntaxTreeNode * op = a -> right;
        SyntaxTreeNode * b = op -> right;


        // 如果是数字运算的话
        if (cur -> value == "Expression-DoubleOp") {
            string a_place = _expression(a);
            string b_place = _expression(b);

            string temp_var_place = "t" + int2string(++ temp_var_index);
            _emit(Quadruple::INTER_CODE_MAP[op -> first_son -> value], a_place, b_place, temp_var_place);

            return temp_var_place;
        }
        // bool运算要考虑拉链回填
        else {
            string temp_op = op -> first_son -> value;

            if (temp_op == "&&") {
                string a_place = _expression(a, "N");
                zipper_stack.push(make_pair(inter_code.size() - 1, 1));

                string b_place = _expression(b, "N");
                zipper_stack.push(make_pair(inter_code.size() - 1, 2));
            }
            else if (temp_op == "||") {
                string a_place = _expression(a, "P");
                zipper_stack.push(make_pair(inter_code.size() - 1, 1));

                string b_place = _expression(b, "N");
                // 回填一下
                zipperFill(inter_code.size() - 1);

                zipper_stack.push(make_pair(inter_code.size() - 1, 1));
            }
            else {
                string a_place = _expression(a);
                string b_place = _expression(b);
                zipper_stack.push(make_pair(inter_code.size(), 1));

                if (previous == "P")
                    _emit(Quadruple::INTER_CODE_MAP[temp_op], a_place, b_place, "");
                else
                    _emit(Quadruple::COUNTERPART_INTER_CODE_MAP[temp_op], a_place, b_place, "");
            }


            return "";
        }
    }
    // 单目运算符
    else if (cur -> value == "Expression-UniOp") {
        // TODO
    }
    else if (cur -> value == "Expression-Bool-UniOp"){
        // TODO
    }
    // 常量
    else if (cur -> value == "Expression-Constant"){
        return cur -> first_son -> value;
    }
    // 字符串常量
    else if (cur -> value == "Expression-String") {
        string temp = cur -> first_son -> value;
        // 转义
        temp = regex_replace(temp, regex(","), "\\,");
        temp = regex_replace(temp, regex("\\\\"), "\\\\");

        return temp;
    }
    // 变量
    else if (cur -> value == "Expression-Variable") {
        return _lookUp(cur -> first_son -> value);
    }
    // 数组项
    else if (cur -> value == "Expression-ArrayItem") {
        return _lookUp(cur);
    }

    cout << "debug >> " << cur -> value << endl;
    throw Error("How can you step into this place???");
}


/**
 * @brief 翻译变量声明语句
 * @author Keyi Li
 */
void InterCodeGenerator::_statement(SyntaxTreeNode * cur) {
    SyntaxTreeNode * cs = cur -> first_son;

    while (cs) {
        string type = cs -> type;
        if (type == "double" || type == "float") {
            Info info(VARIABLE_INFO_ENUM::DOUBLE, ++ var_index);
            table[cs -> value] = info;
        }
        else if (type == "int") {
            Info info(VARIABLE_INFO_ENUM::INT, ++ var_index);
            table[cs -> value] = info;
        }
        else if (type.size() > 6 && type.substr(0, 6) == "array-") {
            Info info(VARIABLE_INFO_ENUM::ARRAY, ++ var_index);
            table[cs -> value] = info;

            string extra_info = cs -> extra_info;
            int extra_info_len = extra_info.size();

            int cur_i = 0;
            if (cur_i < extra_info_len && extra_info.substr(cur_i, 5) == "size=") {
                cur_i += 5;
                int len = 0;
                while (cur_i + len < extra_info_len && extra_info[cur_i + len] != '&')
                    len ++;

                var_index += string2int(extra_info.substr(cur_i, len));
                cur_i += len;
            }
            if (cur_i < extra_info_len && extra_info.substr(cur_i, 3) == "&v=") {
                cur_i += 3;

                int len, arr_i = 0;
                while (cur_i < extra_info_len) {
                    len = 0;
                    while (cur_i + len < extra_info_len && extra_info[cur_i + len] != ',')
                        len ++;

                    _emit(INTER_CODE_OP_ENUM::MOV,
                            extra_info.substr(cur_i, len),
                            "",
                            info.name + "[" + int2string(arr_i) + "]");

                    cur_i += len + 1;
                    arr_i ++;
                }
            }
        }
        else {
            throw Error("type `" + type + "` are not supported yet");
        }

        cs = cs -> right;
    }
}


/**
 * @brief 寻找标识符
 * @param name 标识符
 * @return code var
 * @author Keyi Li
 */
string InterCodeGenerator::_lookUp(string name) {
    if (table.find(name) == table.end())
        throw Error("variable `" + name + "` is not defined before use");

    return table[name].name;
}


/**
 * @brief 寻找标识符
 * @param name 标识符
 * @return code var
 * @author Keyi Li
 */
string InterCodeGenerator::_lookUp(SyntaxTreeNode * arr_pointer) {
    string base = arr_pointer -> first_son -> value;
    string index_place = _expression(arr_pointer -> first_son -> right -> first_son);

    return _lookUp(base) + "[" + index_place + "]";
}


/**
 * @brief 生成一个四元式
 * @param op 操作符
 * @param arg1 参数1
 * @param arg2 参数2
 * @param res 结果
 * @author Keyi Li
 */
void InterCodeGenerator::_emit(INTER_CODE_OP_ENUM op, string arg1, string arg2, string res) {
    inter_code.emplace_back(Quadruple(op, move(arg1), move(arg2), move(res)));
}


/**
 * @brief 保存到文件
 * @param 路径
 * @author Keyi Li
 */
void InterCodeGenerator::saveToFile(string path) {
    ofstream out_file;
    out_file.open(path, ofstream::out | ofstream::trunc);
    for (auto ic: inter_code)
        out_file << Quadruple::INTER_CODE_OP[int(ic.op)] << "," << ic.arg1 << "," << ic.arg2 << "," << ic.res << endl;

    out_file.close();
}


/**
 * @brief 数组第i项的地址
 * @author Keyi Li
 */
string InterCodeGenerator::_locateArrayItem(string arr_name, string arr_i) {
    if (table.find(arr_name) == table.end())
        throw Error("array variable `" + arr_name + "` is not defined before use");

    return "v" + int2string(table[arr_name].place) + "[" + arr_i + "]";
}


/**
 * @brief 拉链回填
 * @author Keyi Li
 */
void InterCodeGenerator::zipperFill(int cur) {
    if (zipper_stack.empty()) {
        cout << "debug " << " the zipper stack is empty?" << endl;
        return;
    }

    int cnt = zipper_stack.top().second, temp_idx;
    cout << "total" << cnt << endl;
    while (cnt --) {
        temp_idx = zipper_stack.top().first;
        zipper_stack.pop();

        cout << "to be filled index = " << temp_idx << " , res = " << cur << endl;

        inter_code[temp_idx].res = int2string(cur);
    }

    cout << endl;
}