/**
 * @file syntax_tree.cc
 * @brief 语法树的函数们
 */
#include "../include/syntax_tree.h"

/**
 * @brief 语法树节点构造函数
 */
SyntaxTreeNode::SyntaxTreeNode(string _value, string _type, string _extra_info) {
    left = right = father = first_son = nullptr;

    value = move(_value);
    type = move(_type);
    extra_info = move(_extra_info);

    true_list.clear();
    false_list.clear();
    next_list.clear();
}


SyntaxTreeNode::SyntaxTreeNode(string _value, int _line_number, int _pos) {
    left = right = father = first_son = nullptr;

    value = move(_value);
    line_number = _line_number;
    pos = _pos;

    true_list.clear();
    false_list.clear();
    next_list.clear();
}

SyntaxTreeNode::SyntaxTreeNode(string _value, string _type, string _extra_info, int _line_number, int _pos) {
    left = right = father = first_son = nullptr;

    value = move(_value);
    type = move(_type);
    extra_info = move(_extra_info);
    line_number = _line_number;
    pos = _pos;

    true_list.clear();
    false_list.clear();
    next_list.clear();
}

/**
 * @brief 语法树构造函数
 */
SyntaxTree::SyntaxTree(SyntaxTreeNode * _root) {
    root = cur_node = _root;
}


/**
 * @brief 悬挂一个节点
 */
void SyntaxTree::addNode(SyntaxTreeNode * child_node, SyntaxTreeNode * father_node) {
    child_node -> father = father_node;

    cur_node = father_node -> first_son;
    if (! cur_node)
        father_node -> first_son = child_node;
    else {
        while (cur_node -> right)
            cur_node = cur_node -> right;

        child_node -> left = cur_node;
        cur_node -> right = child_node;
    }

    cur_node = child_node;
}


/**
 * @brief dfs语法树
 */
 void SyntaxTree::dfs(SyntaxTreeNode * cur, int depth, int status, bool verbose) {
     // windows似乎不支持这些字符集
     // linux、unix 是支持的
     /*
     for (int i = 0; i < depth; i ++) {
         if (status & (1 << (depth - i - 1)))
             cout << "    ";
         else
             cout << "│   ";
     }
     cout << (cur -> right ? "├── " : "└── " ) << cur -> value;
    */

     // 这里的trick是状态压缩，提高速度
     // status 二进制 第i位为1 表示这一层已经结束
     for (int i = 0; i < depth; i ++) {
         if (status & (1 << (depth - i - 1)))
             cout << "    ";
         else
             cout << "|   ";
     }
     cout << (cur -> right ? "|-- " : "\\-- " ) << cur -> value;

     // 除了 value 之外的信息
     if (verbose) {
         if (cur -> type != "" && cur -> extra_info != "")
             cout << " (type: " << cur -> type << ", extra_info: " << cur -> extra_info << ")";
         else if (cur -> type != "")
             cout << " (type: " << cur -> type << ")";
         else if (cur -> extra_info != "")
             cout << " (extra_info: " << cur -> extra_info << ")";

         if (! cur -> true_list.empty()) {
             cout << " true_list = { ";
             for (auto tl: cur -> true_list)
                 cout << tl << ", ";
             cout << "} ";
         }
         if (! cur -> false_list.empty()) {
             cout << " false_list = { ";
             for (auto tl: cur -> false_list)
                 cout << tl << ", ";
             cout << "} ";
         }
         if (! cur -> next_list.empty()) {
             cout << " next_list = { ";
             for (auto tl: cur -> next_list)
                 cout << tl << ", ";
             cout << "} ";
         }
     }

     cout << endl;
     // 如果这是这一层的节点 更新status
     int new_status = (status << 1) + int(cur -> right == nullptr);

     if (cur -> first_son) {
         dfs(cur -> first_son, depth + 1, new_status, verbose);
     }

     if (! cur -> left) {
         while (cur -> right) {
             dfs(cur -> right, depth, status, verbose);
             cur = cur -> right;
         }
     }
 }


/**
 * @brief 打印语法树
 */
 void SyntaxTree::display(bool verbose) {
     cout << root -> value << endl;
     dfs(root -> first_son, 0, 0, verbose);
     cout << endl;
 }
