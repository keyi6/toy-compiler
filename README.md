# awesomeCC
awesome C Compiler

![](https://img.shields.io/travis/cjhahaha/awesomeCC.svg)
![](https://img.shields.io/badge/language-c++-green.svg)
![](https://img.shields.io/badge/license-GPL-blue.svg)


## 项目结构
```
├── awesomeCC
│   ├── CMakeLists.txt
│   ├── error.cc
│   ├── error.h
│   ├── lexical_analyzer.cc
│   ├── lexical_analyzer.h
│   ├── main.cc
│   ├── syntax_analyzer.cc
│   ├── syntax_analyzer.h
│   ├── token.cc
│   └── token.h
├── awesomeCC_test
│   ├── CMakeLists.txt
│   ├── basic_test
└── └── lib
```



1. `main.cc`
   入口文件，处理命令行参数。
2. `error.h`
   错误类，用于报错提示。
3. `token.h`
   Token类，用于记录Token。
4. `lexical_analyzer.h`
   词法分析器类，用于语法分析
5. `syntax_analyzer.h`
   语法分析器类，用于语法分析
6. `test.c`
    用于测试的源文件



## TODO

1. 词法分析

   - [ ] 支持注释

2. 语法分析

   - [x] include 语句

   - [x] 函数声明处理

   - [ ] 函数调用处理

   - [ ] 声明语句

   - [x] 控制语句

      - [ ] if

      - [ ] else

      - [ ] else if

      - [x] while

      - [ ] do

      - [x] for

   - [ ] 赋值语句

   - [ ] 表达式处理

3. 其他
   - [x] Google test

   - [ ] unit test

   - [ ] build test

   - [x] error类直接throw，不存在vector里

