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

#include "../../lib/include/str_tools.h"
#include "../../lib/include/error.h"
#include "../../lib/include/syntax_tree.h"
#include "../include/lexical_analyzer.h"

#include <stack>
#include <iostream>
using std::cout;
using std::endl;
using std::stack;

enum class SENTENCE_PATTERN_ENUM {
    STATEMENT,
    INCLUDE,
    ASSIGNMENT,
    FUNCTION_CALL,
    FUNCTION_STATEMENT,
    CONTROL,
    RETURN,
    RB_BRACKET,
    PRINT,
    ERROR
};


class SyntaxAnalyzer {
private:
    int index, len;
    vector<Token> tokens;
    vector<int> line_number_map;
    SyntaxTree * tree;

    SENTENCE_PATTERN_ENUM _judgeSentencePattern();

    void _analyze();

    void _print(SyntaxTreeNode * father_node);
    void _include(SyntaxTreeNode * father_node);
    void _functionStatement(SyntaxTreeNode * father_node);
    void _statement(SyntaxTreeNode * father_node);
    void _functionCall(SyntaxTreeNode * father_node);
    void _block(SyntaxTreeNode * father_node);
    void _return(SyntaxTreeNode * father_node);
    void _expression(SyntaxTreeNode * father_node, TOKEN_TYPE_ENUM stop_token = TOKEN_TYPE_ENUM::SEMICOLON);
    void _assignment(SyntaxTreeNode * father_node, TOKEN_TYPE_ENUM stop_token = TOKEN_TYPE_ENUM::SEMICOLON);
    void _control(SyntaxTreeNode * father_node);
    void _for(SyntaxTreeNode * father_node);
    void _while(SyntaxTreeNode * father_node);
    void _if(SyntaxTreeNode * father_node);
    void _else(SyntaxTreeNode * father_node);
    void _else_if(SyntaxTreeNode * father_node);

public:
    SyntaxAnalyzer();
    bool analyze(vector<string> sentences, bool verbose = true);
    SyntaxTree * getSyntaxTree();
};


#endif //AWESOMECC_SYNTAX_ANALYZER_HPP
