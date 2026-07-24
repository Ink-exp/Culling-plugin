cmake_minimum_required(VERSION 3.18.1)
project("culling_plugin")

set(CMAKE_CXX_STANDARD 17)

# Tell the compiler to look inside src/ for headers
include_directories(src)

# Fetch Dobby automatically
include(FetchContent)
FetchContent_Declare(
    dobby
    GIT_REPOSITORY https://github.com/jmpews/Dobby.git
    GIT_TAG master
)
FetchContent_MakeAvailable(dobby)

add_library(
    culling_plugin
    SHARED
    src/main.cpp
)

find_library(log-lib log)

# Link Dobby to our plugin
target_link_libraries(
    culling_plugin
    dobby
    ${log-lib}
)
