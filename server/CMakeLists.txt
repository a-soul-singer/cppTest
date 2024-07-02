cmake_minimum_required(VERSION 3.5.0)
project(server VERSION 0.1.0 LANGUAGES C CXX)

include_directories(
    /usr/include/mysql
    platform/include    
    opensource/jsoncpp/jsoncpp-1.9.5/include
    opensource/fmt/fmt-10.2.0/include
)

add_subdirectory(opensource/jsoncpp/jsoncpp-1.9.5)
add_subdirectory(opensource/fmt/fmt-10.2.0)


link_directories(opensource/jsoncpp/jsoncpp-1.9.5/build/lib)

add_executable(
    server 
    platform/src/server.cpp
    platform/src/db_util.cpp
    main.cpp
)

target_link_libraries(
    server 
    jsoncpp 
    pthread
    mysqlclient
    fmt
)