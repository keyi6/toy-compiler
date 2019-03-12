# 将 front end 的文件加入
aux_source_directory(front-end/include F_SRC)
aux_source_directory(front-end/src F_INC)

# 将 back end 的文件加入
aux_source_directory(back-end/include B_SRC)
aux_source_directory(back-end/src B_INC)

# 将 lib 的文件加入
aux_source_directory(lib/src LIB_SRC)
aux_source_directory(lib/include LIB_INC)


# 生成静态库文件
add_library(
        llcc_lib
        all_api.h
        ${LIB_SRC} ${LIB_INC}
        ${F_SRC} ${F_INC}
        ${B_SRC} ${B_INC}
)


# 生成可执行文件 llcc
add_executable(
        llcc
        main.cc
)

target_link_libraries(
        llcc
        llcc_lib
)
