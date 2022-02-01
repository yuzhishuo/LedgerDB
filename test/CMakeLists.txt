cmake_minimum_required(VERSION 3.0.0)
project(moontest)

find_package(GTest CONFIG REQUIRED)

include_directories(../include)
aux_source_directory(. TEST_DIR_SRCS)

add_executable(moontest ${TEST_DIR_SRCS})
target_link_libraries(moontest PRIVATE 
    GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main
)