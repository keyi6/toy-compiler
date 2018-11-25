/**
 *
 * @file syntax_analyzer.hpp
 * @brief 语法分析器类具体实现
 *
 * @author Keyi Li
 *
 */

#include "syntax_analyzer.hpp"

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
 * @brief 语法分析器构造函数
 */
SyntaxAnalyzer::SyntaxAnalyzer() {
}



/**
 * @brief 进行语法分析
 */
// TODO: analyze
void SyntaxAnalyzer::analyze() {
    index = 0;
    tree.cur_node = tree.root = new SyntaxTreeNode("Sentence");

}




/**
 * @brief 处理申明语句
 */
void SyntaxAnalyzer::_statement() {

}