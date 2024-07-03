cmake_minimum_required(VERSION 3.0.0)
project(server VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    /usr/include/mysql
    platform/include    
    opensource/jsoncpp/jsoncpp-1.9.5/include
    opensource/fmt/fmt-10.2.0/include
)

add_subdirectory(opensource/jsoncpp/jsoncpp-1.9.5)
add_subdirectory(opensource/fmt/fmt-10.2.0)

link_directories(opensource/jsoncpp/jsoncpp-1.9.5/build/lib)

aux_source_directory(platform/src SRC_CODE)

add_executable(
    server 
    ${SRC_CODE}
)
# 设置目标程序编译后的位置
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")

target_link_libraries(
    server 
    jsoncpp 
    pthread
    mysqlclient
    fmt
    crypto
)