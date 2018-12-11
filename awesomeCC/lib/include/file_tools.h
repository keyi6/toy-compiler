/**
 * @file file_tools.h
 * @brief 处理文件的函数们
 */

#ifndef AWESOMECC_FILE_TOOLS_H
#define AWESOMECC_FILE_TOOLS_H

#include "quadruple.h"
#include "str_tools.h"

#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;
using std::regex;
using std::vector;
using std::string;
using std::ifstream;
using std::regex_replace;


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
        cout << "File error" << endl;
        cout << "no file named `" << path << "`" << endl;
        exit(0);
    }

    return ret;
}



/**
 * @brief 读取中间代码文件
 * @param path 文件路径
 * @return vector<string> 句子数组
 */
vector<Quadruple> readInterCodeFile(string path) {
    ifstream in_file;
    in_file.open(path);

    vector<Quadruple> ret;

    if (in_file.is_open()) {
        string line;
        while (getline(in_file, line)) {
            if (line.size() <= 3) {
                break;
            }

            int start = 0, len = 0, line_len = line.size();
            while (start + len < line_len && line[start + len] != ',')
                len ++;

            INTER_CODE_OP_ENUM op = Quadruple::INTER_CODE_MAP[line.substr(start, len)];

            start += len + 1;
            len = 0;
            while (start + len < line_len && ! (line[start + len] == ',' && line[start + len - 1] != '\\'))
                len ++;
            string arg1 = regex_replace(line.substr(start, len), regex("\\\\"), string(""));

            start += len + 1;
            len = 0;
            while (start + len < line_len && line[start + len] != ',')
                len ++;
            string arg2 = line.substr(start, len);

            start += len + 1;
            len = 0;
            while (start + len < line_len)
                len ++;
            string res = line.substr(start, len);

            ret.emplace_back(Quadruple(op, arg1, arg2, res));
        }
    }
    else {
        cout << "File error" << endl;
        cout << "no file named `" << path << "`" << endl;
        exit(0);
    }

    return ret;
}

#endif //AWESOMECC_FILE_TOOLS_H
