# 将awesomeCC文件夹下的所有文件存到ACC_SOURCE中
aux_source_directory(${PROJECT_SOURCE_DIR}/awesomeCC ACC_MAIN)
aux_source_directory(${PROJECT_SOURCE_DIR}/awesomeCC/include ACC_SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/awesomeCC/src ACC_INCLUDE)

# 生成可执行文件 awesomeCC_exe
add_executable( ${PROJECT_NAME}_exe ${ACC_MAIN} ${ACC_SOURCE} ${ACC_INCLUDE} include/str_tools.h include/file_tools.h)

# 生成库文件 awesomeCC_lib
add_library( ${PROJECT_NAME}_lib ${ACC_MAIN} ${ACC_SOURCE} ${ACC_INCLUDE} include/str_tools.h include/file_tools.h)

