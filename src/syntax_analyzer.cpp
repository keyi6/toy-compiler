/**
 *
 * @file syntax_analyzer.hpp
 * @brief 语法分析器类具体实现
 *
 * @author Keyi Li
 *
 */

#include "syntax_analyzer.hpp"

/* SyntaxTreeNode */
SyntaxTreeNode::SyntaxTreeNode() {
    left = right = father = firstSon = NULL;
}



/* SyntaxTree */
SyntaxTree::SyntaxTree() {
    root = curNode = NULL;
}




/* SyntaxAnalyzer */
// TODO: constructor
SyntaxAnalyzer::SyntaxAnalyzer() {

}



// TODO: analyze
void SyntaxAnalyzer::analyze() {

}



void SyntaxAnalyzer::_statement() {

}