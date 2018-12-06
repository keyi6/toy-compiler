/**
 *
 * @file encapsulation.cc
 * @brief lib入口文件
 *
 * @author Keyi Li
 *
 */
#ifndef AWESOMECC_ENCAPSULATION_H
#define AWESOMECC_ENCAPSULATION_H


#include "lib/include/file_tools.h"
#include "front-end/include/lexical_analyzer.h"
#include "front-end/include/syntax_analyzer.h"
#include "front-end/include/inter_code_generator.h"
#include "back-end/include/interpreter.h"


/**
 * @brief 词法分析，输出token
 * @param path 代码文件路径
 * @author Keyi Li
 */
void lexer(string path) {
    vector<string> source_file = readSourceFile(path);

    LexicalAnalyzer la;
    la.analyze(source_file);
}


/**
 * @brief 语法分析，输出token 和 语法树
 * @param path 代码文件路径
 * @author Keyi Li
 */
void phraser(string path) {
    vector<string> source_file = readSourceFile(path);

    SyntaxAnalyzer sa;
    sa.analyze(source_file);
}


/**
 * @brief 语义分析 & 中间代码生成，输出好看的中间代码，并生成.ic（inter code）文件
 * @param path 代码文件路径
 * @author Keyi Li
 */
void code_generator(string path) {
    vector<string> source_file = readSourceFile(path);

    SyntaxAnalyzer sa;
    sa.analyze(source_file, false);

    InterCodeGenerator icg;
    icg.analyze(sa.getSyntaxTree(), true);
    icg.saveToFile(path + ".ic");
}


/**
 * @brief 解释执行中间代码
 * @param path 中间代码文件路径
 * @author Keyi Li
 */
void interpreter(string path) {
    vector<Quadruple> inter_code_file = readInterCodeFile(path);

    Interpreter intp;
    intp.execute(inter_code_file);
}


#endif //AWESOMECC_ENCAPSULATION_H
