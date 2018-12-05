/**
 *
 * @file syntax_tree.cc
 * @brief 语法树的函数们
 *
 * @author Keyi Li
 *
 */
#include "../include/syntax_tree.h"

/**
 * @brief 语法树节点构造函数
 * @author Keyi Li
 */
SyntaxTreeNode::SyntaxTreeNode(string _value, string _type, string _extra_info) {
    left = right = father = first_son = nullptr;
    value = _value;
    type = _type;
    extra_info = _extra_info;
}


/**
 * @brief 语法树构造函数
 * @author Keyi Li
 */
SyntaxTree::SyntaxTree(SyntaxTreeNode * _root) {
    root = cur_node = _root;
}


/**
 * @brief 悬挂一个节点
 * @author Keyi Li
 */
void SyntaxTree::addChildNode(SyntaxTreeNode * child_node, SyntaxTreeNode * father_node) {
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
 * @brief 交换左右两个相邻节点
 * @author Keyi Li
 */
void SyntaxTree::switchNode(SyntaxTreeNode * left, SyntaxTreeNode * right) {
    SyntaxTreeNode * ll = left -> left, * rr = right -> right;

    left -> left = right;
    left -> right = rr;

    right -> right = ll;
    right -> right = left;

    if (ll)
        ll -> right = right;
    if (rr)
        rr -> left = left;
}


/**
 * @brief dfs语法树
 * @author Keyi Li
 */
 void SyntaxTree::dfs(SyntaxTreeNode * cur, int depth, int status, bool verbose) {
     for (int i = 0; i < depth; i ++) {
         if (status & (1 << (depth - i - 1)))
             cout << "    ";
         else
             cout << "│   ";
     }
     cout << (cur -> right ? "├── " : "└── " ) << cur -> value;

     if (verbose) {
         if (cur -> type != "" && cur -> extra_info != "")
             cout << " (type: " << cur -> type << ", extra_info: " << cur -> extra_info << ")";
         else if (cur -> type != "")
             cout << " (type: " << cur -> type << ")";
         else if (cur -> extra_info != "")
             cout << " (extra_info: " << cur -> extra_info << ")";
     }

     cout << endl;
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
 * @author Keyi Li
 */
 void SyntaxTree::display(bool verbose) {
     cout << root -> value << endl;
     dfs(root -> first_son, 0, 0, verbose);
     cout << endl;
 }
