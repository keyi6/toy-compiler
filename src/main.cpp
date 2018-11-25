/**
 *
 * @file main.cpp
 * @brief 入口文件
 *
 * @author Keyi Li
 *
 */
#include "lexical_analyzer.hpp"

#include <iostream>
using namespace std;

int main() {
    // TEST
    LexicalAnalyzer a;
    vector<string> v;
    v.emplace_back("#inlude <stdio.h>");
    v.emplace_back("int main() {");
    v.emplace_back("}");

    a.analyze(v);
}