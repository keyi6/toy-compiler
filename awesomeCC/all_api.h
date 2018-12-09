//
// Created by cj on 2018-12-09.
//

#ifndef AWESOMECC_ALL_API_H
#define AWESOMECC_ALL_API_H

#include "front-end/include/syntax_analyzer.h"
#include "front-end/include/inter_code_generator.h"
#include "back-end/include/interpreter.h"
#include "lib/include/file_tools.h"


/**
 * @brief 解释执行中间代码
 * @param path 中间代码文件路径
 * @author Keyi Li
 */
inline void compile_and_execute(string path) {
    vector<string> source_file = readSourceFile(path);

    SyntaxAnalyzer sa;
    sa.analyze(source_file, false);

    InterCodeGenerator icg;
    icg.analyze(sa.getSyntaxTree(), false);
    icg.saveToFile(path + ".ic");

    vector<Quadruple> inter_code_file = readInterCodeFile(path + ".ic");
    Interpreter intp;
    intp.execute(inter_code_file);
}
#endif //AWESOMECC_ALL_API_H
