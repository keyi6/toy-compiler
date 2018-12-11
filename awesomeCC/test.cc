/**
 * @file test.cc
 * @brief 测试入口文件
 */

#include "front-end/frontend_api.h"
#include "back-end/backend_api.h"


int main() {
    string path = "/Users/cjhahaha/Workspace/CompilePrinciple/awesomeCC/demo/qsort.ac";
    vector<string> source_file = readSourceFile(path);

    SyntaxAnalyzer sa;
    sa.analyze(source_file, false);


    sa.getSyntaxTree() -> display(true);
    InterCodeGenerator icg;
    icg.analyze(sa.getSyntaxTree(), true);
    icg.saveToFile(path + ".ic");


    Interpreter itep;
    vector<Quadruple> code = readInterCodeFile(path + ".ic");
    itep.execute(code);//, true);
}