/**
 *
 * @file syntax_analyzer.h
 * @brief 语法分析器类，用于生成语法树
 *
 * @author Keyi Li
 *
 */
#ifndef AWESOMECC_SYNTAX_ANALYZER_HPP
#define AWESOMECC_SYNTAX_ANALYZER_HPP

#include "../../lib/include/token.h"
#include "../../lib/include/error.h"


enum class SENTENCE_PATTERN_ENUM {
    STATEMENT,
    INCLUDE,
    ASSIGNMENT,
    FUNCTION_CALL,
    FUNCTION_STATEMENT,
    CONTROL,
    RETURN,
    RB_BRACKET,
    ERROR
};


class SyntaxTreeNode {
public:
    string value, type, extra_info;

    // 孩子-兄弟表示法
    // left 是左兄弟, right 是右边兄弟
    // father 是父节点, first_son 是第一个子节点
    SyntaxTreeNode * left, * right, * father, * first_son;

    SyntaxTreeNode(string _value = "", string _type = "", string _extra_info = "");
};


class SyntaxTree {
private:
    void dfs(SyntaxTreeNode * cur, int depth, int status);

public:
    SyntaxTreeNode * root, * cur_node;
    SyntaxTree(SyntaxTreeNode * _root = nullptr);

    void addChildNode(SyntaxTreeNode * child_node, SyntaxTreeNode * father_node = nullptr);
    void switchNode(SyntaxTreeNode * left, SyntaxTreeNode * right);
    void display();
};


class SyntaxAnalyzer {
private:
    int index, len;
    vector<Token> tokens;
    vector<int> line_number_map;
    SyntaxTree * tree;

    SENTENCE_PATTERN_ENUM _judgeSentencePattern();

    void _analyze();

    void _include(SyntaxTreeNode * father_node);
    void _functionStatement(SyntaxTreeNode * father_node);
    void _statement(SyntaxTreeNode * father_node);
    void _functionCall(SyntaxTreeNode * father_node);
    void _block(SyntaxTreeNode * father_node);
    void _return(SyntaxTreeNode * father_node);
    void _expression(SyntaxTreeNode * father_node);
    void _assignment(SyntaxTreeNode * father_node);
    void _control(SyntaxTreeNode * father_node);
    void _for(SyntaxTreeNode * father_node);
    void _while(SyntaxTreeNode * father_node);
    void _if(SyntaxTreeNode * father_node);
    void _else(SyntaxTreeNode * father_node);
    void _else_if(SyntaxTreeNode * father_node);

public:
    SyntaxAnalyzer();
    bool analyze(vector<string> sentences, bool verbose = true);
};


#endif //AWESOMECC_SYNTAX_ANALYZER_HPP
