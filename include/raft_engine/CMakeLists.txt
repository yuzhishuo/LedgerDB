add_library(raft_engine 
    RaftEngine.cpp
    net1/RaftServer.cpp
    net1/RaftServer.imp.cpp
    net1/uv_helper.cc
)

# gcov: https://www.cnblogs.com/turtle-fly/archive/2013/01/06/2847190.html
# how to use : https://blog.csdn.net/yanxiangyfg/article/details/80989680
target_link_libraries(raft_engine gcov)

target_link_libraries(
    raft_engine
    gen
    utility
    rocksdb
    raft
    uv_a
    fmt
)

