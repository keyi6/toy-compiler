/**
 * @file syntax_analyzer.cc
 * @brief 语法分析器类具体实现
 */

#include "../include/syntax_analyzer.h"
#define POS(x) x.line_number, x.pos



/**
 * @brief 语法分析器构造函数
 */
SyntaxAnalyzer::SyntaxAnalyzer() = default;


/**
 * @brief 进行语法分析
 * @param sentences string vector, 等待分析的句子们
 * @param verbose bool, 是否输出语法树
 */
void SyntaxAnalyzer::analyze(vector<string> sentences, bool verbose) {
    LexicalAnalyzer la;
    // 如果能通过词法分析
    la.analyze(sentences, true);

    index = 0;
    tokens = la.getAllTokens();
    len = tokens.size();

    try {
        _analyze();
    }
    catch (Error & e) {
        cout << "Syntax analyze errors" << endl;
        cout << e;
        exit(0);
    }

    if (verbose)
        tree -> display();
}


/**
 * @brief 进行语法分析
 */
void SyntaxAnalyzer::_analyze() {
    tree = new SyntaxTree(new SyntaxTreeNode("Class-" + tokens[1].value, POS(tokens[index])));

    // 对语句们分开处理
    while (index < len) {
        int sentence_pattern = int(_judgeSentencePattern());

        switch (sentence_pattern) {
            case int(SENTENCE_PATTERN_ENUM::STATEMENT):
                _statement(tree -> root);
                break;
            case int(SENTENCE_PATTERN_ENUM::FUNCTION_STATEMENT):
                _functionStatement(tree -> root);
                break;
            case int(SENTENCE_PATTERN_ENUM::FUNCTION_CALL):
                _functionCall(tree -> cur_node);
                break;
            case int(SENTENCE_PATTERN_ENUM::PRINT):
                _print(tree -> cur_node);
                break;
            default:
                throw Error("in main, unidentified symbol", POS(tokens[index]));
        }
    }
}


/**
 * @brief 判断句子的种类
 * @return SENTENCE_PATTERN_ENUM, 句子种类的枚举类
 */
SENTENCE_PATTERN_ENUM SyntaxAnalyzer::_judgeSentencePattern() {
    int token_type = int(tokens[index].type);
    string token_value = tokens[index].value;

    switch (token_type) {
        // print 语句
        case int(TOKEN_TYPE_ENUM::PRINT):
            return SENTENCE_PATTERN_ENUM::PRINT;
            // include 语句
        case int(TOKEN_TYPE_ENUM::SHARP):
            if (index + 1 < len && tokens[index + 1].type == TOKEN_TYPE_ENUM::INCLUDE)
                return SENTENCE_PATTERN_ENUM::INCLUDE;
            // 控制语句
        case int(TOKEN_TYPE_ENUM::IF):
        case int(TOKEN_TYPE_ENUM::ELSE):
        case int(TOKEN_TYPE_ENUM::DO):
        case int(TOKEN_TYPE_ENUM::WHILE):
        case int(TOKEN_TYPE_ENUM::FOR):
            return SENTENCE_PATTERN_ENUM::CONTROL;
            // 函数声明 或者 变量申明
        case int(TOKEN_TYPE_ENUM::INT):
        case int(TOKEN_TYPE_ENUM::FLOAT):
        case int(TOKEN_TYPE_ENUM::DOUBLE):
        case int(TOKEN_TYPE_ENUM::CHAR):
        case int(TOKEN_TYPE_ENUM::VOID):
            if (index + 2 < len && tokens[index + 1].type == TOKEN_TYPE_ENUM::IDENTIFIER) {
                TOKEN_TYPE_ENUM nn_type = tokens[index + 2].type;
                if (nn_type == TOKEN_TYPE_ENUM::SEMICOLON ||  // int a;
                    nn_type == TOKEN_TYPE_ENUM::LM_BRACKET || // int a[10];
                    nn_type == TOKEN_TYPE_ENUM::COMMA)        // int a, b;
                    return SENTENCE_PATTERN_ENUM::STATEMENT;
                else if (nn_type == TOKEN_TYPE_ENUM::LL_BRACKET)
                    return SENTENCE_PATTERN_ENUM::FUNCTION_STATEMENT;
            }
            // 函数调用 或者 赋值语句
        case int(TOKEN_TYPE_ENUM::IDENTIFIER):
            if (index + 1 < len) {
                TOKEN_TYPE_ENUM n_type = tokens[index + 1].type;
                if (n_type == TOKEN_TYPE_ENUM::ASSIGN || n_type == TOKEN_TYPE_ENUM::LM_BRACKET)        // sum = 10
                    return SENTENCE_PATTERN_ENUM::ASSIGNMENT;
                if (n_type == TOKEN_TYPE_ENUM::LL_BRACKET)    // sum(10);
                    return SENTENCE_PATTERN_ENUM::FUNCTION_CALL;
            }
            // return 语句
        case int(TOKEN_TYPE_ENUM::RETURN):
            return SENTENCE_PATTERN_ENUM::RETURN;
            // 右大括号 表示函数 或者 block的结束
        case int(TOKEN_TYPE_ENUM::RB_BRACKET):
            return SENTENCE_PATTERN_ENUM::RB_BRACKET;
        default:
            return SENTENCE_PATTERN_ENUM::ERROR;
    }
}


/**
 * @brief 处理print语句
 */
void SyntaxAnalyzer::_print(SyntaxTreeNode * father_node) {
    SyntaxTree * print_tree = new SyntaxTree(new SyntaxTreeNode("Print", POS(tokens[index])));
    tree -> addNode(print_tree -> root, father_node);

    // 读取 print
    index ++;

    // 读取 (
    if (tokens[index].type != TOKEN_TYPE_ENUM::LL_BRACKET)
        throw Error("in print function, arguments should be wrapped in `()`");
    index ++;

    // 找 ）
    int temp_end;
    while (index < len && tokens[index].type != TOKEN_TYPE_ENUM::RL_BRACKET) {
        temp_end = index;
        while (temp_end < len &&
               tokens[temp_end].type != TOKEN_TYPE_ENUM::RL_BRACKET &&
               tokens[temp_end].type != TOKEN_TYPE_ENUM::COMMA)
            temp_end ++;

        // 如果是字符串
        if (tokens[index].type == TOKEN_TYPE_ENUM::DOUBLE_QUOTE) {
            // 读取 "
            index ++;

            // 存值
            print_tree -> addNode(new SyntaxTreeNode("Expression-String", POS(tokens[index])),
                                                     print_tree -> root);
            print_tree -> addNode(new SyntaxTreeNode("\"" + tokens[index].value + "\"", POS(tokens[index])),
                                                     print_tree -> cur_node);

            // 读取 "
            index ++;
            if (tokens[index].type != TOKEN_TYPE_ENUM::DOUBLE_QUOTE)
                throw Error("excepted `\"` appears in pairs", POS(tokens[index]));

            index ++;
        }
        // 如果是变量 或者 表达式
        else {
            _expression(print_tree -> root, tokens[temp_end].type);
        }

        // 检查右括号
        if (tokens[temp_end].type == TOKEN_TYPE_ENUM::RL_BRACKET) {
            index ++;
            break;
        }
    }

}


/**
 * @brief 处理申明语句
 */
void SyntaxAnalyzer::_statement(SyntaxTreeNode * father_node) {
    SyntaxTree * state_tree = new SyntaxTree(new SyntaxTreeNode("Statement", POS(tokens[index])));
    tree -> addNode(state_tree -> root, father_node);

    // 读取变量类型
    string variable_type = tokens[index].value;
    index ++;

    // 找结尾
    string cur_value;
    int cur_type;
    while (index < len && tokens[index].type!= TOKEN_TYPE_ENUM::SEMICOLON) {
        cur_value = tokens[index].value, cur_type = int(tokens[index].type);

        switch (cur_type) {
            // 是个标识符
            case int(TOKEN_TYPE_ENUM::IDENTIFIER): {
                index ++;

                TOKEN_TYPE_ENUM n_type = tokens[index].type;
                // 如果是，或者；就直接读取
                if (n_type == TOKEN_TYPE_ENUM::COMMA || n_type == TOKEN_TYPE_ENUM::SEMICOLON) {
                    state_tree -> addNode(new SyntaxTreeNode(cur_value, variable_type, "", POS(tokens[index])),
                                          state_tree -> root);
                    index ++;

                    if (n_type == TOKEN_TYPE_ENUM::COMMA)
                        break;
                    else
                        return;
                }
                // 如果是[，就按照数组定义来做
                if (n_type == TOKEN_TYPE_ENUM::LM_BRACKET) {
                    // 读取 [
                    index ++;

                    // 读取数组大小
                    string size = "size=" + tokens[index].value;
                    index ++;

                    // 读取 ]
                    if (tokens[index].type == TOKEN_TYPE_ENUM::RM_BRACKET) {
                        index ++;

                        n_type = tokens[index].type;

                        // 如果是，或者；就直接读取
                        if (n_type == TOKEN_TYPE_ENUM::COMMA || n_type == TOKEN_TYPE_ENUM::SEMICOLON) {
                            state_tree -> addNode(new SyntaxTreeNode(cur_value, "array-" + variable_type, size, POS(tokens[index])),
                                                  state_tree -> root);

                            if (tokens[index ++].type == TOKEN_TYPE_ENUM::COMMA)
                                break;
                            else
                                return;
                        }
                        // 如果是 = 那么就读取那些初始化信息
                        else if (n_type == TOKEN_TYPE_ENUM::ASSIGN) {
                            index ++;

                            TOKEN_TYPE_ENUM nn_type = tokens[index].type;
                            // 如果是 {
                            if (nn_type == TOKEN_TYPE_ENUM::LB_BRACKET) {
                                // 读取 {
                                index ++;

                                // 用cookie的格式来存info
                                string init_v = "&v=";
                                do {
                                    if (tokens[index].type == TOKEN_TYPE_ENUM::DIGIT_CONSTANT)
                                        init_v += tokens[index].value;

                                    index ++;
                                    if (tokens[index].type == TOKEN_TYPE_ENUM::RB_BRACKET)
                                        break;
                                    if (tokens[index].type == TOKEN_TYPE_ENUM::COMMA) {
                                        init_v += ",";
                                        index ++;
                                    }
                                    else
                                        throw Error( "in array initialization, expected `,` or `}` after a digital constant",
                                                     POS(tokens[index]));
                                } while (index < len && tokens[index].type != TOKEN_TYPE_ENUM::RB_BRACKET);

                                index ++;
                                n_type = tokens[index].type;
                                if (n_type == TOKEN_TYPE_ENUM::COMMA || n_type == TOKEN_TYPE_ENUM::SEMICOLON) {
                                    state_tree -> addNode(new SyntaxTreeNode(cur_value, "array-" + variable_type, size + init_v, POS(tokens[index])),
                                                          state_tree -> root);
                                    if (tokens[index ++].type == TOKEN_TYPE_ENUM::COMMA)
                                        break;
                                    else
                                        return;
                                }
                            }
                            else
                                throw Error("in array initialization, expected `{}`", POS(tokens[index]));
                        }
                        else
                            throw Error("in statement, unrecognized symbol `" + tokens[index].value + "`", POS(tokens[index]));
                    }
                    else
                        throw Error("in statement, expected `]` after a statement of an array", POS(tokens[index]));
                }
                throw Error("in statement, Unrecognized symbol in statement", POS(tokens[index]));
            }
            default:
                throw Error("in statement, unrecognized symbol `" +  tokens[index].value + "`", POS(tokens[index]));
        }
    }
}


/**
 * @brief 处理表达式
 */
void SyntaxAnalyzer::_expression(SyntaxTreeNode * father_node, TOKEN_TYPE_ENUM stop_sign) {
    stack<SyntaxTree *> op_stack;
    vector<SyntaxTree *> reverse_polish_exp;

    TOKEN_TYPE_ENUM cur_type;
    while (index < len && tokens[index].type != stop_sign) {
        cur_type = tokens[index].type;

        // 常量
        if (cur_type == TOKEN_TYPE_ENUM::DIGIT_CONSTANT) {
            SyntaxTree * new_tree = new SyntaxTree(new SyntaxTreeNode("Expression-Constant", POS(tokens[index])));
            new_tree -> addNode(new SyntaxTreeNode(tokens[index].value,
                                                   POS(tokens[index])),
                                new_tree -> root);

            reverse_polish_exp.emplace_back(new_tree);
            index ++;
        }
        // 变量
        else if (cur_type == TOKEN_TYPE_ENUM::IDENTIFIER) {
            // 数组下标
            if (index + 3 < len && tokens[index + 1].type == TOKEN_TYPE_ENUM::LM_BRACKET) {
                SyntaxTree * new_tree = new SyntaxTree(new SyntaxTreeNode("Expression-ArrayItem", POS(tokens[index])));

                // 数组名字
                new_tree -> addNode(new SyntaxTreeNode(tokens[index].value, POS(tokens[index])),
                                    new_tree -> cur_node);

                // 读取 名字 和 [
                index += 2;

                // 数组下标
                SyntaxTreeNode * index_node = new SyntaxTreeNode("Array-Index", POS(tokens[index]));
                new_tree -> addNode(index_node, new_tree -> root);
                _expression(index_node, TOKEN_TYPE_ENUM::RM_BRACKET);

                reverse_polish_exp.emplace_back(new_tree);
            }
            // 一般的变量
            else {
                SyntaxTree * new_tree = new SyntaxTree(new SyntaxTreeNode("Expression-Variable", POS(tokens[index])));
                new_tree -> addNode(new SyntaxTreeNode(tokens[index].value,
                                                       POS(tokens[index])),
                                    new_tree -> root);

                reverse_polish_exp.emplace_back(new_tree);
                index ++;
            }
        }
        // 运算符
        else if (Token::isExpressionOperator(cur_type)) {
            SyntaxTree * new_tree = new SyntaxTree(new SyntaxTreeNode("Expression-Operator", POS(tokens[index])));
            new_tree -> addNode(new SyntaxTreeNode(tokens[index].value,
                                                   POS(tokens[index])), new_tree -> root);

            // 如果是 (
            if (cur_type == TOKEN_TYPE_ENUM::LL_BRACKET) {
                op_stack.push(new_tree);
            }
            // 如果是 ）
            else if (cur_type == TOKEN_TYPE_ENUM::RL_BRACKET) {
                SyntaxTree * temp_t;
                bool flag = false;
                while (! op_stack.empty()) {
                    temp_t = op_stack.top();
                    op_stack.pop();

                    if (temp_t -> root -> first_son -> value == token2string(TOKEN_TYPE_ENUM::LL_BRACKET)) {
                        flag = true;
                        break;
                    }

                    reverse_polish_exp.emplace_back(temp_t);
                }

                if (! flag)
                    throw Error("in expression, expected `(` before `)`", POS(tokens[index]));
            }
            // 如果是其他的 + - * / > < 那些
            else {
                SyntaxTree * temp_t;
                int cur_pio = int(tokens[index].type), t_pio;
                while (! op_stack.empty()) {
                    temp_t = op_stack.top();
                    t_pio = temp_t -> root -> first_son -> value == "(" ? -1 :
                            int(Token::DETAIL_TOKEN_TYPE[temp_t -> root -> first_son -> value]);

                    if (t_pio <= cur_pio)
                        break;

                    reverse_polish_exp.emplace_back(temp_t);
                    op_stack.pop();
                }

                op_stack.push(new_tree);
            }

            index ++;
        }
        else
            throw Error("in expression, unrecognized symbols `" + tokens[index].value + "`" , POS(tokens[index]));
    }

    if (!(len < index || tokens[index].type == stop_sign))
        throw Error("in expression, expected token `" + token2string(stop_sign) + "` at the end", POS(tokens[index]));

    // 读取stop sign
    index ++;

    SyntaxTree * temp_t;
    while (! op_stack.empty()) {
        temp_t = op_stack.top();
        if (temp_t -> root -> first_son -> value == "(")
            throw Error("in expression, expected `)` after `(`", POS(tokens[index]));
        reverse_polish_exp.emplace_back(temp_t);
        op_stack.pop();
    }

    // 这里把op_stack 做成操作数栈
    SyntaxTree * a, * b;
    int temp_size = reverse_polish_exp.size();
    for (int i = 0; i < temp_size; i ++) {
        temp_t = reverse_polish_exp[i];

        // 如果是运算符
        if (temp_t -> root -> value == "Expression-Operator") {
            // 如果是单目运算符
            if (Token::isUniOperator(Token::DETAIL_TOKEN_TYPE[temp_t -> root -> first_son -> value])) {
                a = op_stack.top();
                op_stack.pop();

                SyntaxTree * new_tree;
                if (Token::isBoolOperator(Token::DETAIL_TOKEN_TYPE[temp_t -> root -> first_son -> value])) {
                    new_tree = new SyntaxTree(new SyntaxTreeNode( "Expression-Bool-UniOp", POS(tokens[index])));
                }
                else {
                    new_tree = new SyntaxTree(new SyntaxTreeNode( "Expression-UniOp", POS(tokens[index])));
                }

                // 添加操作符
                new_tree -> addNode(temp_t -> root, new_tree -> root);
                // 添加操作数
                new_tree -> addNode(a -> root, new_tree -> root);

                op_stack.push(new_tree);
            }
            // 如果是双目运算符
            else {
                b = op_stack.top();
                op_stack.pop();

                a = op_stack.top();
                op_stack.pop();

                SyntaxTree * new_tree;
                if (Token::isBoolOperator(Token::DETAIL_TOKEN_TYPE[temp_t -> root -> first_son -> value])) {
                    new_tree = new SyntaxTree(new SyntaxTreeNode( "Expression-Bool-DoubleOp", POS(tokens[index])));
                }
                else {
                    new_tree = new SyntaxTree(new SyntaxTreeNode( "Expression-DoubleOp", POS(tokens[index])));
                }

                if (Token::isBoolOperator(Token::DETAIL_TOKEN_TYPE[temp_t -> root -> first_son -> value])) {
                    string temp_op = temp_t -> root -> first_son -> value;
                    if (temp_op == ">=") {
                        temp_t -> root -> first_son -> value = "<";
                        swap(a, b);
                    }
                    else if (temp_op == "<=") {
                        temp_t -> root -> first_son -> value = ">";
                        swap(a, b);
                    }
                }

                // 添加操作数
                new_tree -> addNode(a -> root, new_tree -> root);
                // 添加操作符
                new_tree -> addNode(temp_t -> root, new_tree -> root);
                // 添加操作数
                new_tree -> addNode(b -> root, new_tree -> root);

                op_stack.push(new_tree);
            }
        }
        // 不是运算符
        else
            op_stack.push(temp_t);
    }

    temp_t = op_stack.top();
    tree -> addNode(temp_t -> root, father_node);
}


/**
 * @brief 处理函数声明
 */
void SyntaxAnalyzer::_functionStatement(SyntaxTreeNode * father_node) {
    SyntaxTree * func_state_tree = new SyntaxTree(new SyntaxTreeNode("FunctionStatement", POS(tokens[index])));
    tree -> addNode(func_state_tree -> root, father_node);

    string cur_value;
    TOKEN_TYPE_ENUM cur_type;

    // 读取返回类型
    func_state_tree -> addNode(new SyntaxTreeNode("Type", POS(tokens[index])), func_state_tree -> root);
    func_state_tree -> addNode(new SyntaxTreeNode(tokens[index].value, POS(tokens[index])), func_state_tree -> cur_node);
    index ++;

    // 读取函数名
    func_state_tree -> addNode(new SyntaxTreeNode("FunctionName", POS(tokens[index])), func_state_tree -> root);
    func_state_tree -> addNode(new SyntaxTreeNode(tokens[index].value, POS(tokens[index])), func_state_tree -> cur_node);
    index ++;

    // 读取(
    index ++;

    // 建一个参数树
    SyntaxTreeNode * param_list = new SyntaxTreeNode("ParameterList", POS(tokens[index]));
    func_state_tree -> addNode(param_list, func_state_tree -> root);

    // 如果下一个是）
    if (tokens[index].type == TOKEN_TYPE_ENUM::RL_BRACKET) {
        index ++;
    }
    // 如果下一个不是），读取参数列表
    else {
        while (index < len && tokens[index].type != TOKEN_TYPE_ENUM::RL_BRACKET) {
            cur_value = tokens[index].value;

            if (cur_value == "int" || cur_value == "double" || cur_value == "float") {
                SyntaxTreeNode * param = new SyntaxTreeNode("Parameter", POS(tokens[index]));
                func_state_tree -> addNode(param, param_list);

                index ++;
                if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::IDENTIFIER) {
                    func_state_tree -> addNode(new SyntaxTreeNode(tokens[index].value,
                                                                  cur_value, "",
                                                                  POS(tokens[index])), param);
                    index ++;

                    if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::COMMA)
                        index ++;
                    else if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::RL_BRACKET) {
                        index ++;
                        break;
                    }
                    else
                        throw Error("in function statement's parameter list, should be `,` or `)` after", POS(tokens[index]));
                }
            }
            else
                throw Error("in function statement's parameter list, unidentified parameter type found", POS(tokens[index]));
        }
    }


    // 如果下一个是 { , 就处理大括号里的内容
    cur_type = tokens[index].type;
    if (cur_type == TOKEN_TYPE_ENUM::LB_BRACKET) {
        _block(func_state_tree -> root);
    }
    // 如果下一个是; ，就当作单纯的函数声明
    // 如果两个都不是 就有问题
    else if (cur_type != TOKEN_TYPE_ENUM::SEMICOLON)
        throw Error("in function statement, expected `;` or `}`", POS(tokens[index]));
}


/**
 * @brief 处理return
 */
void SyntaxAnalyzer::_return(SyntaxTreeNode * father_node) {
    SyntaxTree * return_tree = new SyntaxTree();

    index ++;
    if (index < len) {
        if (tokens[index].type == TOKEN_TYPE_ENUM::SEMICOLON) {
            return_tree -> root = return_tree -> cur_node = new SyntaxTreeNode("VoidReturn", POS(tokens[index]));
            return_tree -> addNode(new SyntaxTreeNode(tokens[index - 1].value, POS(tokens[index])), return_tree -> cur_node);

            tree -> addNode(return_tree -> root, father_node);
            index ++;
        }
        else {
            return_tree -> root = return_tree -> cur_node = new SyntaxTreeNode("Return", POS(tokens[index]));
            tree -> addNode(return_tree -> root, father_node);

            return_tree -> addNode(new SyntaxTreeNode(tokens[index - 1].value, POS(tokens[index])), return_tree -> cur_node);
            _expression(return_tree -> root);
            return;
        }

    }
    else
        throw Error("in return, expected an expression or semicolon after `return`", POS(tokens[index]));
}


/**
 * @brief 处理大括号{} 内的内容
 */
void SyntaxAnalyzer::_block(SyntaxTreeNode * father_node) {
    SyntaxTree * block_tree = new SyntaxTree(new SyntaxTreeNode("Block", POS(tokens[index])));
    tree -> addNode(block_tree -> root, father_node);

    index ++;
    while (index < len && tokens[index].type != TOKEN_TYPE_ENUM::RB_BRACKET) {
        int cur_type = int(_judgeSentencePattern());

        switch (cur_type) {
            case int(SENTENCE_PATTERN_ENUM::STATEMENT):
                _statement(block_tree -> root);
                break;
            case int(SENTENCE_PATTERN_ENUM::ASSIGNMENT):
                _assignment(block_tree -> root);
                break;
            case int(SENTENCE_PATTERN_ENUM::FUNCTION_CALL):
                _functionCall(block_tree -> root);
                break;
            case int(SENTENCE_PATTERN_ENUM::CONTROL):
                _control(block_tree -> root);
                break;
            case int(SENTENCE_PATTERN_ENUM::RETURN):
                _return(block_tree -> root);
                break;
            case int(SENTENCE_PATTERN_ENUM::PRINT):
                _print(block_tree -> root);
                break;
            default:
                // 如果是空语句
                if (tokens[index].type == TOKEN_TYPE_ENUM::SEMICOLON) {
                    index ++;
                    continue;
                }

                throw Error("in block, unidentified symbols `" + tokens[index].value + "`  found",
                            POS(tokens[index]));
        }
    }

    if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::RB_BRACKET)
        index ++;
    else
        throw Error("in block, expected }`", POS(tokens[index]));
}


/**
 * @brief 处理函数调用
 */
void SyntaxAnalyzer::_functionCall(SyntaxTreeNode * father_node) {
    // TODO 在 expression 里添加函数调用

    SyntaxTree * func_call_tree = new SyntaxTree(new SyntaxTreeNode("FunctionCall", POS(tokens[index])));
    tree -> addNode(func_call_tree -> root, father_node);

    func_call_tree -> addNode(new SyntaxTreeNode("FunctionName", POS(tokens[index])), func_call_tree -> root);
    func_call_tree -> addNode(new SyntaxTreeNode(tokens[index].value, POS(tokens[index])), func_call_tree -> cur_node);

    SyntaxTree * param_tree = new SyntaxTree(new SyntaxTreeNode("FunctionParameters", POS(tokens[index])));
    func_call_tree -> addNode(param_tree -> root, func_call_tree -> root);

    // 读取 函数名
    index ++;
    if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::LL_BRACKET) {
        // 读取 （
        index ++;

        int next_end;
        while (index < len && tokens[index].type != TOKEN_TYPE_ENUM::RL_BRACKET) {
            next_end = index;
            while (next_end < len &&
                  (tokens[next_end].type != TOKEN_TYPE_ENUM::RL_BRACKET &&
                   tokens[next_end].type != TOKEN_TYPE_ENUM::COMMA))
                        next_end ++;

            param_tree -> addNode(new SyntaxTreeNode("Param", POS(tokens[index])), param_tree -> root);
            _expression(param_tree -> cur_node, tokens[next_end].type);

            index = next_end + 1;
            if (tokens[next_end].type == TOKEN_TYPE_ENUM::RL_BRACKET)
                break;
        }
    }
    else
        throw Error("in function call, expected `(` after function name",
                    POS(tokens[index]));
}


/**
 * @brief 处理赋值语句
 */
void SyntaxAnalyzer::_assignment(SyntaxTreeNode * father_node, TOKEN_TYPE_ENUM stop_token) {
    SyntaxTree * assign_tree = new SyntaxTree(new SyntaxTreeNode("Assignment", POS(tokens[index])));
    tree -> addNode(assign_tree -> root, father_node);

    if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::IDENTIFIER) {
        assign_tree -> addNode(new SyntaxTreeNode(tokens[index].value, POS(tokens[index])), assign_tree -> root);
        index ++;

        // a[0] = 10;
        if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::LM_BRACKET) {
            assign_tree -> cur_node -> value = "Expression-ArrayItem";

            index ++;
            assign_tree -> addNode(new SyntaxTreeNode(tokens[index - 2].value,
                                                      POS(tokens[index - 2])),
                                   assign_tree -> root -> first_son);
            assign_tree -> addNode(new SyntaxTreeNode("Array-Index", POS(tokens[index])), assign_tree -> root -> first_son);
            _expression(assign_tree -> cur_node, TOKEN_TYPE_ENUM::RM_BRACKET);
        }

        // a = 10;
        if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::ASSIGN) {
            index ++;

            _expression(assign_tree -> root, stop_token);
        }
        else {
            throw Error("in assignment, expected `=` after an identifier", POS(tokens[index]));
        }
    }
    else if (index < len && tokens[index].type == stop_token)
        index ++;
    else
        throw Error("in assignment, expected a `" + token2string(stop_token) + "` after", POS(tokens[index]));
}


/**
 * @brief 处理控制语句
 */
void SyntaxAnalyzer::_control(SyntaxTreeNode * father_node) {
    int cur_type = int(tokens[index].type);
    switch (cur_type) {
        case int(TOKEN_TYPE_ENUM::WHILE):
            _while(father_node);
            break;
        case int(TOKEN_TYPE_ENUM::FOR):
            _for(father_node);
            break;
        case int(TOKEN_TYPE_ENUM::IF):
            _if(father_node);
            break;
        default:
            throw Error("unsupported control statement", POS(tokens[index]));
    }
}


/**
 * @brief 处理for
 */
void SyntaxAnalyzer::_for(SyntaxTreeNode * father_node) {
    SyntaxTree * psudo_while_tree = new SyntaxTree(new SyntaxTreeNode("Control-While", POS(tokens[index])));
    // 读取 for
    index ++;

    if (tokens[index].type == TOKEN_TYPE_ENUM::LL_BRACKET) {
        // 读取 (
        index ++;

        // 读取第一个赋值语句
        // 直接执行
        _assignment(father_node);

        // 读取第二个条件语句
        psudo_while_tree -> addNode(new SyntaxTreeNode("Condition", POS(tokens[index])), psudo_while_tree -> root);
        _expression(psudo_while_tree -> cur_node);

        // 读取第三个赋值语句
        SyntaxTreeNode * temp = new SyntaxTreeNode("", POS(tokens[index]));
        _assignment(temp, TOKEN_TYPE_ENUM::RL_BRACKET);

        // 读取 {
        if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::LB_BRACKET) {
            _block(psudo_while_tree -> root);
            psudo_while_tree -> addNode(temp -> first_son, psudo_while_tree -> root -> first_son -> right);
            tree -> addNode(psudo_while_tree -> root, father_node);
        }
        else
            throw Error("in for, Expected `{` after `for (assignment; condition; assignment)`", POS(tokens[index]));
    }
    else
        throw Error("Expected `(` after `for`", POS(tokens[index]));
}


/**
 * @brief 处理while
 */
void SyntaxAnalyzer::_while(SyntaxTreeNode * father_node) {
    SyntaxTree * while_tree = new SyntaxTree(new SyntaxTreeNode("Control-While", POS(tokens[index])));
    tree -> addNode(while_tree -> root, father_node);

    // 读取while
    index ++;

    if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::LL_BRACKET) {
        index ++;

        // 读取 表达式 直到遇到）
        while_tree -> addNode(new SyntaxTreeNode("Condition", POS(tokens[index])), while_tree -> root);
        _expression(while_tree -> cur_node, TOKEN_TYPE_ENUM::RL_BRACKET);

        // 读取 {
        if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::LB_BRACKET)
            _block(while_tree -> root);
        else
            throw Error("Expected `{` after `while (condition)`", POS(tokens[index]));
    }
    else
        throw Error("Expected `(` after `while`", POS(tokens[index]));
}


/**
 * @brief 处理if
 */
void SyntaxAnalyzer::_if(SyntaxTreeNode * father_node) {
    SyntaxTree * if_tree = new SyntaxTree(new SyntaxTreeNode("Control-If", POS(tokens[index])));
    tree -> addNode(if_tree -> root, father_node);

    // 读取 if
    index ++;

    // 处理if
    if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::LL_BRACKET) {
        // 读取 (
        index ++;

        if_tree -> addNode(new SyntaxTreeNode("Control-Condition", POS(tokens[index])), if_tree -> root);
        _expression(if_tree -> cur_node, TOKEN_TYPE_ENUM::RL_BRACKET);

        // 如果是 {
        if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::LB_BRACKET) {
            _block(if_tree -> root);

            // 如果还有else 和 else if
            if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::ELSE) {
                if (index + 1 < len && tokens[index].type == TOKEN_TYPE_ENUM::IF) {
                    _else_if(if_tree -> root);
                }
                else {
                    _else(if_tree -> root);
                }
            }
        }
        else {
            throw Error("in if, expected `{` after `if (condition)`", POS(tokens[index]));
        }
    }
    else {
        throw Error("in if, expected `(` after `if`", POS(tokens[index]));
    }
}


/**
 * @brief 处理else if
 */
void SyntaxAnalyzer::_else_if(SyntaxTreeNode * father_node) {
    // 读取 else if
    index += 2;

    if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::LL_BRACKET) {
        // 读取 (
        index ++;

        _expression(father_node, TOKEN_TYPE_ENUM::RL_BRACKET);

        // 如果是 {
        if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::LB_BRACKET) {
            _block(father_node);

            // 如果还有else 和 else if
            if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::ELSE) {
                if (index + 1 < len && tokens[index].type == TOKEN_TYPE_ENUM::IF) {
                    _else_if(father_node);
                }
                else {
                    _else(father_node);
                }
            }
        }
        else {
            throw Error("in else-if, expected `{` after `if (condition)`", POS(tokens[index]));
        }
    }
    else {
        throw Error("in else-if, expected `(` after `if`", POS(tokens[index]));
    }
}


/**
 * @brief 处理else
 */
void SyntaxAnalyzer::_else(SyntaxTreeNode * father_node) {
    if (tokens[index].type == TOKEN_TYPE_ENUM::ELSE) {
        index ++;
        if (tokens[index].type == TOKEN_TYPE_ENUM::LB_BRACKET) {
            _block(father_node);
        }
        else {
            throw Error("in if, expected `{` after `else` or `if`", POS(tokens[index]));
        }
    }
    else
        throw Error("in if, how can you do this???", POS(tokens[index]));
}


/**
 * @brief 返回生成的语法🌲
 */
SyntaxTree * SyntaxAnalyzer::getSyntaxTree() {
    return tree;
}
