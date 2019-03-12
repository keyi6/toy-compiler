/**
 * @file main.cc
 * @brief .llacc 运行入口文件，处理命令行参数
 */

#include "front-end/frontend_api.h"
#include "back-end/backend_api.h"
#include "all_api.h"

#include <map>
#include <iostream>

using std::map;
using std::cout;
using std::endl;


map<string, string> OPT = {
        {"-h", "help"},
        {"--help", "help"},
        {"-v", "version"},
        {"--version", "version"},
        {"-o", "output"},
        {"--output", "output"},
        {"-l", "lexer"},
        {"--lexer", "lexer"},
        {"-p", "parser"},
        {"--parser", "parser"},
        {"-a", "assembler"},
        {"--assembler", "assembler"},
        {"-i", "interpreter"},
        {"--interpreter", "interpreter"}
};


map<string, string> HELP_TEXT = {
        {"help", "get help on llcc command line arguments"},
        {"version", "display llcc version"},
        {"output", "the output file path"},
        {"lexer", "lexical analyze a source AC file"},
        {"parser", "syntax analyze a source AC file"},
        {"assembler", "generate inter code(Quadruple) for a source AC file"},
        {"interpreter", "interpret and execute an inter code file"}
};


/**
 * @brief 输出 help text
 */
void getHelp() {
    cout << "OVERVIEW: awesome CC compiler, developed by Keyi Li & Hanwen Liu" << endl << endl;
    cout << "USAGE: acc file_name [options] <inputs>" << endl << endl;

    cout << "OPTIONS:" << endl;
    string display_info;
    string temp_info;
    for (auto it = HELP_TEXT.begin(); it != HELP_TEXT.end(); it ++ ) {
        temp_info = it -> first;
        display_info = "-" + temp_info.substr(0, 1) + ", --" + temp_info;

        cout << setw(30) << setfill(' ') << std::left << display_info;
        cout << it -> second << endl;
    }

    cout << endl;
    cout << "EXAMPLE: " << endl;
    cout << "acc source.ac -l" << endl;
    cout << "acc source.ac -p" << endl;
    cout << "acc source.ac -a" << endl;
    cout << "acc source.ac.ic -i" << endl;
    cout << "acc source.ac" << endl;
    cout << "acc -h" << endl;
    cout << "acc -v" << endl;
}


/**
 * @brief 输出 version text
 */
void getVersion() {
    cout << "awesome CC compiler 1.0" << endl;
    cout << "developed by Keyi Li & Hanwen Liu" << endl;

}


/**
 * @brief 接受命令行参数
 */
int main(int argc, char * argv[]) {
    if (argc <= 1) {
        cout << "please specify an input file!" << endl << endl << endl;
        getHelp();
    }
    else {
        string path = argv[1];

        if (argc == 2) {
            if (path == "-h" || path == "--help")
                getHelp();
            else if (path == "-v" || path == "--version")
                getVersion();
            else
                compile_and_execute(path);
            return 0;
        }
        else {
            for (int i = 2; i < argc; i ++) {
                string opt = argv[i];
                opt = OPT[opt];
                if (opt == "lexer")
                    lexer(path);
                else if (opt == "parser")
                    parser(path);
                else if (opt == "assembler")
                    code_generator(path);
                else if (opt == "interpreter")
                    interpreter(path);
                else {
                    cout << endl << "Error: unknown argument: `" << argv[i] << "`" << endl;
                    return 0;
                }
            }
        }
    }
}
