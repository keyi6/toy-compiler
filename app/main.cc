/**
 *
 * @file main.cc
 * @brief 入口文件
 *
 * @author Keyi Li
 *
 */
#include "../include/lexical_analyzer.h"
#include "../include/syntax_analyzer.h"


#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;


/**
 * @brief 读取源文件
 * @param path 文件路径
 * @return vector<string> 句子数组
 */
vector<string> readSourceFile(string path) {
    ifstream in_file;
    in_file.open(path);

    vector<string> ret;

    if (in_file.is_open()) {
        string line;
        while (getline(in_file, line))
            ret.emplace_back(line);
    }
    else {
        cout << "No such file!" << endl;
    }

    return ret;
}



int main() {
    vector<string> source_file = readSourceFile("../test.c");

    SyntaxAnalyzer sa;
    sa.analyze(source_file);
}