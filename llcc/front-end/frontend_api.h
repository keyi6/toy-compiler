/**
 * @file frontend_api.cc
 * @brief 前段api
 */
#ifndef LLCC_FRONTEND_API_H
#define LLCC_FRONTEND_API_H


#include "../lib/include/file_tools.h"
#include "include/lexical_analyzer.h"
#include "include/syntax_analyzer.h"
#include "include/inter_code_generator.h"


/**
 * @brief 词法分析，输出token
 * @param path 代码文件路径
 */
inline void lexer(string path) {
    vector<string> source_file = readSourceFile(path);

    LexicalAnalyzer la;
    la.analyze(source_file, true);
}


/**
 * @brief 语法分析，输出token 和 语法树
 * @param path 代码文件路径
 */
inline void parser(string path) {
    vector<string> source_file = readSourceFile(path);

    SyntaxAnalyzer sa;
    sa.analyze(source_file, true);
}


/**
 * @brief 语义分析 & 中间代码生成，输出好看的中间代码，并生成.ic（inter code）文件
 * @param path 代码文件路径
 */
inline void code_generator(string path, bool save = true) {
    vector<string> source_file = readSourceFile(path);

    SyntaxAnalyzer sa;
    sa.analyze(source_file, false);

    InterCodeGenerator icg;
    icg.analyze(sa.getSyntaxTree(), false);
    if (save)
        icg.saveToFile(path + ".ic");
}


#endif //LLCC_FRONTEND_API_H
