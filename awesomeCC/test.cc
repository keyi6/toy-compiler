/**
 *
 * @file main.cc
 * @brief 运行入口文件
 *
 * @author Keyi Li
 *
 */

#include "front-end/frontend_api.h"
#include "back-end/backend_api.h"


int main() {
    string path = "/Users/cjhahaha/Workspace/CompilePrinciple/awesomeCC/demo/leap_year.ac";
    vector<string> source_file = readSourceFile(path);

    SyntaxAnalyzer sa;
    sa.analyze(source_file, false);

    sa.getSyntaxTree() -> display(true);

    InterCodeGenerator icg;
    icg.analyze(sa.getSyntaxTree(), true);
    icg.saveToFile(path + ".ic");

    //interpreter(path + ".ic");
}