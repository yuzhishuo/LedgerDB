find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

add_library(gen 
    gen/example.pb.cc
    gen/ledger_engine.pb.cc
    gen/merkle_engine.pb.cc
    gen/query.pb.cc
    gen/raft_engine.pb.cc
    gen/raft.pb.cc
    gen/user_engine.pb.cc

    gen/raft.grpc.pb.cc
    gen/ledger_engine.grpc.pb.cc
)

target_link_libraries(
    gen
    protobuf
    gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++
)