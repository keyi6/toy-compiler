/**
 *
 * @file syntax_analyzer.cc
 * @brief 语法分析器类具体实现
 *
 * @author Keyi Li
 *
 */

#include "syntax_analyzer.h"
#include "lexical_analyzer.h"

#include <queue>
#include <iostream>
using namespace std;


/**
 * @brief 语法树节点构造函数
 */
SyntaxTreeNode::SyntaxTreeNode(string _value, string _type, string _extra_info) {
    left = right = father = first_son = nullptr;
    value = _value;
    type = _type;
    extra_info = _extra_info;
}


/**
 * @brief 语法树构造函数
 */
SyntaxTree::SyntaxTree(SyntaxTreeNode * _root) {
    root = cur_node = _root;
}


/**
 * @brief 悬挂一个节点
 */
void SyntaxTree::addChildNode(SyntaxTreeNode * child_node, SyntaxTreeNode * father_node) {
    child_node -> father = father_node;

    cur_node = father_node -> first_son;
    if (! cur_node)
        father_node -> first_son = child_node;
    else {
        while (cur_node -> right)
            cur_node = cur_node -> right;

        child_node -> left = cur_node;
        cur_node -> right = child_node;
    }

    cur_node = child_node;
}


/**
 * @brief 交换左右两个相邻节点
 */
void SyntaxTree::switchNode(SyntaxTreeNode * left, SyntaxTreeNode * right) {
    SyntaxTreeNode * ll = left -> left, * rr = right -> right;

    left -> left = right;
    left -> right = rr;

    right -> right = ll;
    right -> right = left;

    if (ll)
        ll -> right = right;
    if (rr)
        rr -> left = left;
}


/**
 * @brief dfs语法树
 */
 void SyntaxTree::dfs(SyntaxTreeNode * cur, int depth, int status) {
     for (int i = 0; i < depth; i ++) {
         if (status & (1 << (depth - i - 1)))
             cout << "    ";
         else
             cout << "│   ";
     }
     cout << (cur -> right ? "├── " : "└── " ) << cur -> value << endl;
     int new_status = (status << 1) + int(cur -> right == nullptr);

     if (cur -> first_son) {

         dfs(cur -> first_son, depth + 1, new_status);
     }

     if (! cur -> left) {
         while (cur -> right) {
             dfs(cur -> right, depth, status);
             cur = cur -> right;
         }
     }
 }


/**
 * @brief 打印语法树
 */
 void SyntaxTree::display() {
     cout << root -> value << endl;
     dfs(root -> first_son, 0, 0);
     cout << endl;
 }


/**
 * @brief 语法分析器构造函数
 */
SyntaxAnalyzer::SyntaxAnalyzer() {
}


/**
 * @brief 进行语法分析
 * @param sentences string vector, 等待分析的句子们
 * @param verbose bool, 是否输出语法树
 * @return
 *      -<em>true</em> 无错误
 *      -<em>false</em> 有错误
 */
bool SyntaxAnalyzer::analyze(vector<string> sentences, bool verbose) {
    LexicalAnalyzer la;
    // 如果能通过词法分析
    if (la.analyze(sentences, false)) {
        index = 0;
        tree = new SyntaxTree();

        tokens = la.getAllTokens();
        len = tokens.size();
        line_number_map = la.getLineNumberMap();

        try {
            _analyze();
        }
        catch (Error & e) {
            cout << "Syntax analyze errors" << endl;
            cout << e;
            return false;
        }

        if (verbose)
            tree -> display();

        return true;
    }

    return false;
}


/**
 * @brief 进行语法分析
 */
void SyntaxAnalyzer::_analyze() {
    tree -> cur_node = tree -> root = new SyntaxTreeNode("Program");

    while (index < len) {
        int sentence_pattern = int(_judgeSentencePattern());

        switch (sentence_pattern) {
            case int(SENTENCE_PATTERN_ENUM::INCLUDE):
                _include(tree -> root);
                break;
            case int(SENTENCE_PATTERN_ENUM::STATEMENT):
                _statement(tree -> cur_node);
                break;
            case int(SENTENCE_PATTERN_ENUM::FUNCTION_STATEMENT):
                _functionStatement(tree -> root);
                break;
            case int(SENTENCE_PATTERN_ENUM::FUNCTION_CALL):
                _functionCall(tree -> cur_node);
                break;
            default:
                throw Error("in main, unidentified symbol", line_number_map[index]);
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
        // 申明语句
        case int(TOKEN_TYPE_ENUM::INT):
        case int(TOKEN_TYPE_ENUM::FLOAT):
        case int(TOKEN_TYPE_ENUM::DOUBLE):
        case int(TOKEN_TYPE_ENUM::VOID):
        case int(TOKEN_TYPE_ENUM::CHAR):
            if (index + 2 < len && tokens[index + 1].type == TOKEN_TYPE_ENUM::IDENTIFIER) {
                TOKEN_TYPE_ENUM nn_type = tokens[index + 2].type;
                if (nn_type == TOKEN_TYPE_ENUM::LL_BRACKET)   // int sum();
                    return SENTENCE_PATTERN_ENUM::FUNCTION_STATEMENT;
                if (nn_type == TOKEN_TYPE_ENUM::SEMICOLON ||  // int a;
                    nn_type == TOKEN_TYPE_ENUM::LM_BRACKET || // int a[10];
                    nn_type == TOKEN_TYPE_ENUM::COMMA)        // int a, b;
                    return SENTENCE_PATTERN_ENUM::STATEMENT;
            }
        // 函数调用 或者 赋值语句
        case int(TOKEN_TYPE_ENUM::IDENTIFIER):
            if (index + 1 < len) {
                TOKEN_TYPE_ENUM n_type = tokens[index + 1].type;
                if (n_type == TOKEN_TYPE_ENUM::ASSIGN)        // sum = 10
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
 * @brief 处理申明语句
 */
void SyntaxAnalyzer::_statement(SyntaxTreeNode * father_node) {
    SyntaxTree * state_tree = new SyntaxTree(new SyntaxTreeNode("Statement"));
    tree -> addChildNode(state_tree -> root, father_node);

    // 读取变量类型
    string variable_type = tokens[index].value;
    index ++;

    // TODO 处理声明语句
    /*
    string cur_value;
    int cur_type;
    while (index < len && tokens[index].type != TOKEN_TYPE_ENUM::SEMICOLON) {
        cur_value = tokens[index].value, cur_type = int(tokens[index].type);

        switch (cur_type) {
            case int(TOKEN_TYPE_ENUM::IDENTIFIER):
        }
    }
    */
}


/**
 * @brief 处理表达式
 */
void SyntaxAnalyzer::_expression(SyntaxTreeNode * father_node) {
    SyntaxTree * exp_tree = new SyntaxTree(new SyntaxTreeNode("Expression"));
    tree -> addChildNode(exp_tree -> root, tree -> cur_node);

    // 返回第一个操作数的值，测试用
    exp_tree -> addChildNode(new SyntaxTreeNode(tokens[index].value), exp_tree -> root);
    index ++;

    // TODO 处理表达式
}


/**
 * @brief 处理include语句
 */
void SyntaxAnalyzer::_include(SyntaxTreeNode * father_node) {
    SyntaxTree * include_tree = new SyntaxTree(new SyntaxTreeNode("Include"));

    tree -> addChildNode(include_tree -> root, father_node);

    int quote_cnt = 0;
    bool flag = true;
    while (index < len && flag) {
        if (tokens[index].type == TOKEN_TYPE_ENUM::DOUBLE_QUOTE)
            quote_cnt ++;

        if (quote_cnt == 2 || tokens[index].value == ">")
            flag = false;

        SyntaxTreeNode * new_node = new SyntaxTreeNode(tokens[index].value);
        include_tree -> addChildNode(new_node, include_tree -> root);

        index ++;
    }
}


/**
 * @brief 处理函数声明
 */
void SyntaxAnalyzer::_functionStatement(SyntaxTreeNode * father_node) {
    SyntaxTree * func_state_tree = new SyntaxTree(new SyntaxTreeNode("FunctionStatement"));
    tree -> addChildNode(func_state_tree -> root, father_node);

    string cur_value;
    TOKEN_TYPE_ENUM cur_type;

    // 读取返回类型
    func_state_tree -> addChildNode(new SyntaxTreeNode("Type"), func_state_tree -> root);
    func_state_tree -> addChildNode(new SyntaxTreeNode(tokens[index].value), func_state_tree -> cur_node);
    index ++;

    // 读取函数名
    func_state_tree -> addChildNode(new SyntaxTreeNode("FunctionName"), func_state_tree -> root);
    func_state_tree -> addChildNode(new SyntaxTreeNode(tokens[index].value), func_state_tree -> cur_node);
    index ++;

    // 读取(
    index ++;

    // 如果下一个是）
    if (tokens[index].type == TOKEN_TYPE_ENUM::RL_BRACKET) {
        index ++;
    }
    // 如果下一个不是），读取参数列表
    else {
        // 建一个参数树
        SyntaxTreeNode * param_list = new SyntaxTreeNode("ParameterList");
        func_state_tree -> addChildNode(param_list, func_state_tree -> root);

        while (index < len && tokens[index].type != TOKEN_TYPE_ENUM::RL_BRACKET) {
            cur_value = tokens[index].value;

            if (cur_value == "int" || cur_value == "double" || cur_value == "float") {
                SyntaxTreeNode * param = new SyntaxTreeNode("Parameter");
                func_state_tree -> addChildNode(param, param_list);
                func_state_tree -> addChildNode(new SyntaxTreeNode(cur_value), param);

                index ++;
                if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::IDENTIFIER) {
                    func_state_tree -> addChildNode(new SyntaxTreeNode(tokens[index].value), param);
                    index ++;

                    if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::COMMA)
                        index ++;
                    else if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::RL_BRACKET) {
                        index ++;
                        break;
                    }
                    else
                        throw Error("in function statement's parameter list, should be `,` or `)` after", line_number_map[index]);
                }
            }
            else
                throw Error("in function statement's parameter list, unidentified parameter type found", line_number_map[index]);
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
        throw Error("in function statement, expected `;` or `}`", line_number_map[index]);
}


/**
 * @brief 处理return
 */
void SyntaxAnalyzer::_return(SyntaxTreeNode * father_node) {
    SyntaxTree * return_tree = new SyntaxTree();

    index ++;
    if (index < len) {
        if (tokens[index].type == TOKEN_TYPE_ENUM::SEMICOLON) {
            return_tree -> root = return_tree -> cur_node = new SyntaxTreeNode("VoidReturn");
            return_tree -> addChildNode(new SyntaxTreeNode(tokens[index - 1].value), return_tree -> cur_node);

            tree -> addChildNode(return_tree -> root, father_node);
            index ++;
        }
        else {
            return_tree -> root = return_tree -> cur_node = new SyntaxTreeNode("Return");
            tree -> addChildNode(return_tree -> root, father_node);

            return_tree -> addChildNode(new SyntaxTreeNode(tokens[index - 1].value), return_tree -> cur_node);
            _expression(return_tree -> root);

            if (tokens[index].type == TOKEN_TYPE_ENUM::SEMICOLON)
                index ++;
            else
                throw Error("in return, expected `;`", line_number_map[index]);
            return;
        }

    }
    else
        throw Error("in return, expected an expression or semicolon after `return`", line_number_map[index]);
}


/**
 * @brief 处理大括号
 */
void SyntaxAnalyzer::_block(SyntaxTreeNode * father_node) {
    SyntaxTree * block_tree = new SyntaxTree(new SyntaxTreeNode("Sentence"));
    tree -> addChildNode(block_tree -> root, tree -> cur_node);

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
            default:
                throw Error("in block, unidentified symbols found", line_number_map[index]);
        }
    }

    if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::RB_BRACKET)
        index ++;
    else
        throw Error("in block, expected }`", line_number_map[index]);
}


/**
 * @brief 处理函数调用
 */
void SyntaxAnalyzer::_functionCall(SyntaxTreeNode * father_node) {
    // TODO 处理函数调用
}


/**
 * @brief 处理赋值语句
 */
void SyntaxAnalyzer::_assignment(SyntaxTreeNode *father_node) {
    // TODO 处理赋值语句
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
            throw Error("unsupported control statement", line_number_map[index]);
    }
}


/**
 * @brief 处理for
 */
void SyntaxAnalyzer::_for(SyntaxTreeNode * father_node) {
    // TODO 处理for

}


/**
 * @brief 处理while
 */
void SyntaxAnalyzer::_while(SyntaxTreeNode * father_node) {
    // TODO 处理 while
}


/**
 * @brief 处理if
 */
void SyntaxAnalyzer::_if(SyntaxTreeNode * father_node) {
    // TODO 处理if
}


/**
 * @brief 处理else if
 */
void SyntaxAnalyzer::_else_if(SyntaxTreeNode * father_node) {
    // TODO 处理else if
}


/**
 * @brief 处理else
 */
void SyntaxAnalyzer::_else(SyntaxTreeNode * father_node) {
    // TODO 处理else
}
