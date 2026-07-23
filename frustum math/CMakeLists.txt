cmake_minimum_required(VERSION 3.18.1)
project("culling_plugin")

set(CMAKE_CXX_STANDARD 17)

add_library(
    culling_plugin
    SHARED
    src/main.cpp
)

find_library(log-lib log)

target_link_libraries(
    culling_plugin
    ${log-lib}
)

