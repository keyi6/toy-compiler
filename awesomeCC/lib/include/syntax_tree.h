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

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;
using std::swap;
using std::vector;
using std::string;


class SyntaxTreeNode {
public:
    string value, type, extra_info;

    // 孩子-兄弟表示法
    // left 是左兄弟, right 是右边兄弟
    // father 是父节点, first_son 是第一个子节点
    SyntaxTreeNode * left, * right, * father, * first_son;
    vector<int> true_list, false_list, next_list;

    SyntaxTreeNode(string _value = "", string _type = "", string _extra_info = "");
};


class SyntaxTree {
private:
    void dfs(SyntaxTreeNode * cur, int depth, int status, bool verbose);

public:
    SyntaxTreeNode * root, * cur_node;
    SyntaxTree(SyntaxTreeNode * _root = nullptr);

    void addNode(SyntaxTreeNode * child_node, SyntaxTreeNode * father_node = nullptr);
    void switchNode(SyntaxTreeNode * center);
    void display(bool verbose = false);
};

#endif //AWESOMECC_SYNTAX_TREE_H
