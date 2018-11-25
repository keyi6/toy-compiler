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
    string value, type, extra_info;
    SyntaxTreeNode * left, * right, * father, * first_son;


    SyntaxTreeNode(string _value = "", string _type = "", string _extra_info = "");
};


class SyntaxTree {
public:
    SyntaxTreeNode * root, * cur_node;
    SyntaxTree();
};



class SyntaxAnalyzer {
private:
    int index;
    vector<Token> tokens;
    SyntaxTree tree;

    void _statement();
    /*
     TODO
    void _block();
    void _include();
    void _assignment();
    void _expression();
    void _control();
    void _for();
    void _while();
    void _if();
    void _functionStatement();
    void _functionCall();
    void _return();

    void _judgeSentencePattern();
    */
public:
    SyntaxAnalyzer();
    void analyze();

};


#endif //TOY_C_COMPILER_SYNTAX_ANALYZER_HPP
