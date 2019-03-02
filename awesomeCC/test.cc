/**
 * @file test.cc
 * @brief 测试入口文件，生成awesomeCC_exe，用于调试
 */

#include "front-end/frontend_api.h"
#include "back-end/backend_api.h"


int main() {
    string path = "../../demo/qsort.ac";

    // 读取文件
    vector<string> source_file = readSourceFile(path);

    // 词法分析 并 语法分析
    SyntaxAnalyzer sa;
    sa.analyze(source_file, false);

    // 语义分析 并 生成中间代码
    sa.getSyntaxTree() -> display(true);
    InterCodeGenerator icg;
    icg.analyze(sa.getSyntaxTree(), true);
    icg.saveToFile(path + ".ic");

    // 解释执行
    Interpreter itep;
    vector<Quadruple> code = readInterCodeFile(path + ".ic");
    itep.execute(code);//, true);
}