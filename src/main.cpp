//
// Created by Keyi Li on 2018/11/10.
//

#include "lexical_analyzer.hpp"

#include <iostream>
using namespace std;

int main() {
    // TEST
    LexicalAnalyzer a;
    a.analyze("int a = 134 * (4412 << 2 + 5.4);");
}