/**
 * @file syntax_tree.h
 * @brief 语法树类
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
    int line_number, pos;

    // 孩子-兄弟表示法
    SyntaxTreeNode * left, * right; // left 是左兄弟, right 是右边兄弟
    SyntaxTreeNode * father, * first_son; // father 是父节点, first_son 是第一个子节点
    vector<int> true_list, false_list, next_list;

    SyntaxTreeNode(string _value, string _type = "", string _extra_info = "");
    SyntaxTreeNode(string _value, int _line_number, int _pos);
    SyntaxTreeNode(string _value, string _type, string _extra_info, int _line_number, int _pos);
};


class SyntaxTree {
private:
    void dfs(SyntaxTreeNode * cur, int depth, int status, bool verbose);

public:
    SyntaxTreeNode * root, * cur_node;
    SyntaxTree(SyntaxTreeNode * _root = nullptr);

    void addNode(SyntaxTreeNode * child_node, SyntaxTreeNode * father_node = nullptr);
    void display(bool verbose = false);
};

#endif //AWESOMECC_SYNTAX_TREE_H
