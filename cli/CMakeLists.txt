project(cli VERSION 0.1.0)

find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

include_directories(
    include
    ../include
)

add_executable(cli main.cpp ${SRCS} )
add_dependencies(cli parser )

target_link_libraries(cli
    gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++
)

target_link_libraries(cli
    gen
    readline
    rudeconfig
    parser
    fmt::fmt
)