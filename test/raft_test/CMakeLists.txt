cmake_minimum_required(VERSION 3.0.0)
project(raft_test)


add_executable(raft_test main.cpp)


target_link_libraries(raft_test gcov)
target_link_libraries(raft_test 
raft_engine
spdlog::spdlog spdlog::spdlog_header_only
raft
fmt
rudeconfig
rocksdb
)