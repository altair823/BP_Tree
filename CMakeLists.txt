cmake_minimum_required(VERSION 3.22)
project(bp_tree)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(test)

add_executable(bp_tree main.cpp data_structure/index_node.h data_structure/data_node.h data_structure/data.h tree/bp_tree.h result/result.h tree/solve.h)
target_include_directories(bp_tree PUBLIC data_structure)
target_include_directories(bp_tree PUBLIC result)
