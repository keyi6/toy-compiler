# awesomeCC
awesome C Compiler

![](https://img.shields.io/travis/cjhahaha/awesomeCC.svg)
![](https://img.shields.io/badge/language-c++-green.svg)
![](https://img.shields.io/badge/license-GPL-blue.svg)


## TODO
这个代码太丑了 我要重构一下

1. 语法分析
   - [x] 函数调用处理
   - [x] Expression-Bool

3. 其他
   - [x] unit test
   - [ ] build test
   - [ ] 堆栈的存储区🤔
     - [ ] 内存监控🤔
     - [x] 一块静态区（常量？简单变量？） + 一块动态区（变量）（有递归的都要动态🤔？）
       - [x] 每个block分配一个新的存储区
   - [ ] rewrite travis.yml
   - [ ] write INSTALLATION
   - [ ] 规范化Error
   - [x] Error精准到char pos
