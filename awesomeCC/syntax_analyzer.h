/**
 *
 * @file syntax_analyzer.h
 * @brief 语法分析器类，用于生成语法树
 *
 * @author Keyi Li
 *
 */
#ifndef TOY_C_COMPILER_SYNTAX_ANALYZER_HPP
#define TOY_C_COMPILER_SYNTAX_ANALYZER_HPP

#include "token.h"
#include "error.h"


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

    // left 是左兄弟, right 是右边兄弟
    // father 是父节点, first_son 是第一个子节点
    SyntaxTreeNode * left, * right, * father, * first_son;

    SyntaxTreeNode(string _value = "", string _type = "", string _extra_info = "");
};


class SyntaxTree {
public:
    SyntaxTreeNode * root, * cur_node;
    SyntaxTree();

    void addChildNode(SyntaxTreeNode * child_node, SyntaxTreeNode * father_node = nullptr);
    void switchNode(SyntaxTreeNode * left, SyntaxTreeNode * right);
    void display();
};


class SyntaxAnalyzer {
private:
    int index, len;
    vector<Token> tokens;
    vector<Error> errors;
    SyntaxTree * tree;

    SENTENCE_PATTERN_ENUM _judgeSentencePattern();

    void _analyze();

    void _include(SyntaxTreeNode * father_node);
    void _functionStatement(SyntaxTreeNode * father_node);
    void _functionCall();
    void _statement(SyntaxTreeNode * father_node);

    void _block(SyntaxTreeNode * father_node);
    /*
     TODO
    void _assignment();
    void _expression();
    void _control();
    void _for();
    void _while();
    void _if();
    void _return();
    */

public:
    SyntaxAnalyzer();
    bool analyze(vector<string> sentences, bool verbose = true);
};


#endif //TOY_C_COMPILER_SYNTAX_ANALYZER_HPP
