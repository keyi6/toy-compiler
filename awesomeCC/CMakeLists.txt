# 将awesomeCC文件夹下的所有文件存到ACC_SOURCE中

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
        ${LIB_SRC} ${LIB_INC}
        ${F_SRC} ${F_INC}
        ${B_SRC} ${B_INC}
        main.cc
        encapsulation.h
)

# 生成库文件 awesomeCC_lib
add_library(
        ${PROJECT_NAME}_lib
        ${LIB_SRC} ${LIB_INC}
        ${F_SRC} ${F_INC}
        ${B_SRC} ${B_INC}
        encapsulation.h
)

