/**
 * @file backend_api.h
 * @brief 后端api
 */

#ifndef AWESOMECC_BACKEND_API_H
#define AWESOMECC_BACKEND_API_H

#include "include/interpreter.h"
#include "../lib/include/file_tools.h"

/**
 * @brief 解释执行中间代码
 * @param path 中间代码文件路径
 */
inline void interpreter(string path) {
    vector<Quadruple> inter_code_file = readInterCodeFile(path);

    Interpreter intp;
    intp.execute(inter_code_file);
}



#endif //AWESOMECC_BACKEND_API_H
