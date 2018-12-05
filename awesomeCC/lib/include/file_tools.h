/**
 *
 * @file file_tools.h
 * @brief 处理文件的函数们
 *
 * @author Keyi Li
 *
 */

#ifndef AWESOMECC_FILE_TOOLS_H
#define AWESOMECC_FILE_TOOLS_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using std::vector;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;

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

#endif //AWESOMECC_FILE_TOOLS_H
