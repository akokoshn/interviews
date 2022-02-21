cmake_minimum_required(VERSION 3.14)

project(dict)

set(SRC_FILES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

add_executable(dict
    ${SRC_FILES}
)

target_compile_options(dict PUBLIC -std=c++11)
