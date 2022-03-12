project(Cli VERSION 0.1.0)


find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

include_directories(
   include
)

add_executable(Cli main.cpp ${SRCS} )

target_link_libraries(Cli
    gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++

)

target_link_libraries(Cli
    gen
)