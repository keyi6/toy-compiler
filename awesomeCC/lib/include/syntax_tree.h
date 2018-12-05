/**
 *
 * @file syntax_tree.h
 * @brief 语法树类
 *
 * @author Keyi Li
 *
 */
#ifndef AWESOMECC_SYNTAX_TREE_H
#define AWESOMECC_SYNTAX_TREE_H

#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;


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
    void dfs(SyntaxTreeNode * cur, int depth, int status, bool verbose);

public:
    SyntaxTreeNode * root, * cur_node;
    SyntaxTree(SyntaxTreeNode * _root = nullptr);

    void addChildNode(SyntaxTreeNode * child_node, SyntaxTreeNode * father_node = nullptr);
    void switchNode(SyntaxTreeNode * left, SyntaxTreeNode * right);
    void display(bool verbose = false);
};

#endif //AWESOMECC_SYNTAX_TREE_H
