cmake_minimum_required(VERSION 3.27.0)
project(client VERSION 0.1.0 LANGUAGES C CXX)

include_directories(
    opensource/jsoncpp/include
    platform/include
    )
link_directories(opensource/jsoncpp/lib)

add_executable(client client.cpp)

target_link_libraries(client jsoncpp)
