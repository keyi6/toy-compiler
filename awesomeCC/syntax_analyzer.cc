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
SyntaxTree::SyntaxTree() {
    root = cur_node = nullptr;
}


/**
 * @brief 悬挂一个节点
 */
void SyntaxTree::addChildNode(SyntaxTreeNode * child_node, SyntaxTreeNode * father_node) {
    if (! father_node)
        father_node = cur_node;

    child_node -> father = father_node;
    if (! father_node -> first_son)
        father_node -> first_son = child_node;
    else {
        cur_node = father_node -> first_son;
        while (cur_node -> right)
            cur_node = cur_node -> right;

        cur_node -> right = child_node;
        child_node -> left = cur_node;
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
 * @brief 打印语法树
 */
 void SyntaxTree::display() {
     int cur_depth, pre_depth = 0;
     SyntaxTreeNode * cur, * node, * pre_father = nullptr;
     queue< pair<SyntaxTreeNode * , int> > q;

     q.push(make_pair(root, 1));

     while (not q.empty()) {
         cur = q.front().first;
         cur_depth = q.front().second;
         q.pop();

         if (pre_depth < cur_depth) {
             cout << endl << endl << "depth #" << cur_depth << endl;
             pre_depth = cur_depth;
             pre_father = cur -> father;
         }
         if (cur -> father != pre_father) {
             cout << "---" << endl;
             pre_father = cur -> father;
         }

         cout << "Type: " << cur -> type << "    Value: " << cur -> value << endl;

         node = cur -> first_son;
         while (node) {
             q.push(make_pair(node, cur_depth + 1));
             node = node -> right;
         }
     }
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

        _analyze();

        if (verbose)
            tree -> display();

        // TODO 错误处理
        if (! errors.empty()) {
            if (verbose) {
                cout << "Errors\n";
                for (auto e: errors)
                    cout << e;
            }
        }
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
                _functionCall();
                break;
            default:
                errors.emplace_back(Error("Errors in main"));
                return;
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
    // TODO
}



/**
 * @brief 处理include语句
 */
void SyntaxAnalyzer::_include(SyntaxTreeNode * father_node) {
    SyntaxTree * include_tree = new SyntaxTree();
    include_tree -> root = include_tree -> cur_node = new SyntaxTreeNode("INCLUDE");

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

        index += 1;
    }
}


/**
 * @brief 处理函数声明
 */
void SyntaxAnalyzer::_functionStatement(SyntaxTreeNode * father_node) {
    SyntaxTree * func_state_tree = new SyntaxTree();
    func_state_tree -> root = func_state_tree -> cur_node = new SyntaxTreeNode("FunctionStatement");

    tree -> addChildNode(func_state_tree -> root, father_node);

    string cur_value;
    TOKEN_TYPE_ENUM cur_type;
    while (index < len) {
        cur_value = tokens[index].value;
        cur_type = tokens[index].type;

        // 如果是返回值类型
        if (cur_value == "int" || cur_value == "double" || cur_value == "float" || cur_value == "void") {
            func_state_tree -> addChildNode(new SyntaxTreeNode("Type"), func_state_tree -> root);
            func_state_tree -> addChildNode(new SyntaxTreeNode(cur_value), func_state_tree -> cur_node);
            index += 1;
        }
        // 如果是函数名
        else if (cur_type == TOKEN_TYPE_ENUM::IDENTIFIER) {
            func_state_tree -> addChildNode(new SyntaxTreeNode("FunctionName"), func_state_tree -> root);
            func_state_tree -> addChildNode(new SyntaxTreeNode(cur_value), func_state_tree -> cur_node);
            index += 1;
        }
        // 如果是参数列表的(
        else if (cur_type == TOKEN_TYPE_ENUM::LL_BRACKET) {
            SyntaxTreeNode * param_list = new SyntaxTreeNode("ParameterList");
            func_state_tree -> addChildNode(param_list, func_state_tree -> root);
            index += 1;

            while (index < len && tokens[index].type != TOKEN_TYPE_ENUM::RL_BRACKET) {
                cur_value = tokens[index].value;

                if (cur_value == "int" || cur_value == "double" || cur_value == "float" || cur_value == "void") {
                    SyntaxTreeNode * param = new SyntaxTreeNode("Parameter");
                    func_state_tree -> addChildNode(param, param_list);
                    func_state_tree -> addChildNode(new SyntaxTreeNode(cur_value), param);

                    index += 1;
                    if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::IDENTIFIER) {
                        func_state_tree -> addChildNode(new SyntaxTreeNode(tokens[index].value), param);
                        index += 1;

                        if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::COMMA)
                            index += 1;
                        else if (index < len && tokens[index].type == TOKEN_TYPE_ENUM::RL_BRACKET) {
                            index += 1;
                            break;
                        }
                        else {
                            errors.emplace_back(Error("should be , ) after"));
                            return;
                        }
                    }
                }
                else {
                    errors.emplace_back(Error("wrong in parameter list"));
                    return;
                }
            }
        }
        // 如果是{
        else if (cur_type == TOKEN_TYPE_ENUM::LB_BRACKET) {
            _block(func_state_tree -> root);
        }
        else {
            errors.emplace_back(Error("unidentified symbol in parameter list"));
            return;
        }
    }
}


/**
 * @brief 处理函数调用
 */
void SyntaxAnalyzer::_functionCall() {
    // TODO
}



/**
 * @brief 处理大括号
 */
void SyntaxAnalyzer::_block(SyntaxTreeNode * father_node) {
    
}