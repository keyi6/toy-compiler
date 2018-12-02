/**
 *
 * @file main.cc
 * @brief 入口文件
 *
 * @author Keyi Li
 *
 */
#include "include/file_tools.h"
#include "include/lexical_analyzer.h"
#include "include/syntax_analyzer.h"

#include <vector>
#include <string>
using std::string;
using std::vector;


int main() {
    vector<string> source_file = readSourceFile("../../test.c");

    SyntaxAnalyzer sa;
    sa.analyze(source_file);
}