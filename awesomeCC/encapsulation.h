//
// Created by cj on 2018/12/5.
//

#ifndef AWESOMECC_ENCAPSULATION_H
#define AWESOMECC_ENCAPSULATION_H


#include "lib/include/file_tools.h"
#include "front-end/include/lexical_analyzer.h"
#include "front-end/include/syntax_analyzer.h"
#include "front-end/include/inter_code_generator.h"
#include "back-end/include/interpreter.h"


// TODO 注释
void lexer(string path) {
    vector<string> source_file = readSourceFile(path);

    LexicalAnalyzer la;
    la.analyze(source_file);
}


void phraser(string path) {
    vector<string> source_file = readSourceFile(path);

    SyntaxAnalyzer sa;
    sa.analyze(source_file);
}


void code_generator(string path) {
    vector<string> source_file = readSourceFile(path);

    SyntaxAnalyzer sa;
    sa.analyze(source_file, false);

    InterCodeGenerator icg;
    icg.analyze(sa.getSyntaxTree(), true);
    icg.saveToFile(path + ".ic");
}


void interpreter(string path) {
    vector<Quadruple> inter_code_file = readInterCodeFile(path);

    Interpreter intp;
    intp.execute(inter_code_file);
}


#endif //AWESOMECC_ENCAPSULATION_H
