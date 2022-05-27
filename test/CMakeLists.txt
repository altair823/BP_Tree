project(Google_tests)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../data_structure)
include_directories(../tree)
include_directories(../logger)

add_executable(Google_Tests_run index_node_test.cpp data_node_test.cpp bp_tree_test.cpp test_util.h)
target_link_libraries(Google_Tests_run gtest gtest_main)

add_executable(execution_time_test execution_time_test.cpp)