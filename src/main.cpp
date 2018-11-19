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
    //a.analyzeSentence("# include\"stdio\"");
    vector<string> v;
    v.emplace_back("#include <stdio.h>");
    v.emplace_back("int main() {");
    v.emplace_back("}");
    a.analyze(v);

}