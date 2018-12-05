/**
 *
 * @file main.cc
 * @brief 入口文件
 *
 * @author Keyi Li
 *
 */
#include "lib/include/file_tools.h"
#include "front-end/include/lexical_analyzer.h"
#include "front-end/include/syntax_analyzer.h"

#include <vector>
#include <string>
using std::string;
using std::vector;


int main() {
    vector<string> source_file = readSourceFile("../../test.ac");

    SyntaxAnalyzer sa;
    sa.analyze(source_file);
}