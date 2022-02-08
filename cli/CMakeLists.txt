project(Cli VERSION 0.1.0)


find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_executable(Cli main.cpp ) 

target_link_libraries(Cli
gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++
)