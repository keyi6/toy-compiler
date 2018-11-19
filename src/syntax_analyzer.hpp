/**
 *
 * @file syntax_analyzer.hpp
 * @brief 语法分析器类，用于生成语法树
 *
 * @author Keyi Li
 *
 */
#ifndef TOY_C_COMPILER_SYNTAX_ANALYZER_HPP
#define TOY_C_COMPILER_SYNTAX_ANALYZER_HPP

#include "token.hpp"

class SyntaxTreeNode {
public:
    Token t;
    SyntaxTreeNode * left, * right, * father, * firstSon;


    SyntaxTreeNode();

};


class SyntaxTree {
private:
    SyntaxTreeNode * root, * curNode;


public:
    SyntaxTree();
};


class SyntaxAnalyzer {
private:
    int index;
    SyntaxTree tree;

    void _statement();

public:
    SyntaxAnalyzer();
    void analyze();

};


#endif //TOY_C_COMPILER_SYNTAX_ANALYZER_HPP
