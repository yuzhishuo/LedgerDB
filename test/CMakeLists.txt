cmake_minimum_required(VERSION 3.0.0)
project(moontest)

include_directories(../include)
aux_source_directory(. TEST_DIR_SRCS)

add_executable(moontest ${TEST_DIR_SRCS})
target_link_libraries(moontest PRIVATE 
    GTest::gmock
    GTest::gtest
    GTest::gtest_main
    utility
)

add_subdirectory(raft_test)