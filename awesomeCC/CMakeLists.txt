# front end
aux_source_directory(front-end/include F_SRC)
aux_source_directory(front-end/src F_INC)

# back end
aux_source_directory(back-end/include B_SRC)
aux_source_directory(back-end/src B_INC)

# lib
aux_source_directory(lib/src LIB_SRC)
aux_source_directory(lib/include LIB_INC)



# 生成可执行文件 awesomeCC_exe
add_executable(
        ${PROJECT_NAME}_exe
        test.cc
        ${LIB_SRC} ${LIB_INC}
        ${F_SRC} ${F_INC}
        ${B_SRC} ${B_INC}
        encapsulation.h
)

# 生成可执行文件 acc
add_executable(
        acc
        main.cc
        encapsulation.h
        ${LIB_SRC} ${LIB_INC}
        ${F_SRC} ${F_INC}
        ${B_SRC} ${B_INC}
)