/**
 *
 * @file main.cc
 * @brief 运行入口文件
 *
 * @author Keyi Li
 *
 */

#include "encapsulation.h"


int main() {
    string path = "../../test.ac";
    vector<string> source_file = readSourceFile(path);

    SyntaxAnalyzer sa;
    sa.analyze(source_file, false);

    InterCodeGenerator icg;
    icg.analyze(sa.getSyntaxTree(), true);
    icg.saveToFile(path + ".ic");

    sa.getSyntaxTree() -> display(true);


    interpreter("../../test.ac.ic");
}