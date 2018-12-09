//
// Created by cj on 2018-12-09.
//

#ifndef AWESOMECC_ALL_API_H
#define AWESOMECC_ALL_API_H

#include "front-end/include/syntax_analyzer.h"
#include "front-end/include/inter_code_generator.h"
#include "back-end/include/interpreter.h"
#include "lib/include/file_tools.h"

#include <ctime>
#include <iostream>
using std::cout;
using std::endl;

/**
 * @brief 解释执行中间代码
 * @param path 中间代码文件路径
 * @author Keyi Li
 */
inline void compile_and_execute(string path) {
    vector<string> source_file = readSourceFile(path);

    time_t start_time = time(nullptr);
    cout << "start compiling " << path << "..." << endl << endl;

    SyntaxAnalyzer sa;
    sa.analyze(source_file, false);

    InterCodeGenerator icg;
    icg.analyze(sa.getSyntaxTree(), false);
    icg.saveToFile(path + ".ic");

    time_t end_time = time(nullptr);
    cout << "compile finish in " << (end_time - start_time) << " sec(s)." << endl << endl;

    cout << "------ start executing ------" << endl;
    vector<Quadruple> inter_code_file = readInterCodeFile(path + ".ic");
    Interpreter intp;
    intp.execute(inter_code_file);
}
#endif //AWESOMECC_ALL_API_H
