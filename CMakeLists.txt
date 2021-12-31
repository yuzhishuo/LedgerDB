cmake_minimum_required(VERSION 3.0.0)
project(example_casbin_cpp VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(RocksDB CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(Threads REQUIRED)

message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection )
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

file(GLOB PROTO_FILES protos/*.proto)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cpp)

set(_GRPC_GRPCPP gRPC::grpc++ )
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin> )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LOCAL_W_FLAGS} ${LOCAL_C_FLAGS} -fPIC")

set(SRCS "" CACHE FILEPATH "" FORCE )
foreach(OBJ ${PROTO_FILES})
    message(STATUS "Compiling ${OBJ}")
    get_filename_component(OBJNAME ${OBJ} NAME_WLE)
    set(proto_srcs ${CMAKE_CURRENT_BINARY_DIR}/cpp/${OBJNAME}.pb.cc)
    set(proto_hdrs ${CMAKE_CURRENT_BINARY_DIR}/cpp/${OBJNAME}.pb.h)
    list(APPEND SRCS ${proto_srcs})

    set(grpc_srcs ${CMAKE_CURRENT_BINARY_DIR}/cpp/${OBJNAME}.grpc.pb.cc)
    set(grpc_hdrs ${CMAKE_CURRENT_BINARY_DIR}/cpp/${OBJNAME}.grpc.pb.h)
    list(APPEND SRCS ${grpc_srcs})
    message(STATUS "Compiling ${_PROTOBUF_PROTOC}")
    add_custom_command(
      OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}/cpp"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/cpp"
        -I "${CMAKE_CURRENT_LIST_DIR}/protos"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${OBJ}"
      DEPENDS "${OBJ}")
endforeach()

include_directories(./build/cpp)

include_directories(./include)
aux_source_directory(./src DIR_SRCS)
add_executable(samplate-cpp-template ${DIR_SRCS} ${SRCS} main.cpp )

target_link_libraries(samplate-cpp-template
                    casbin
                    RocksDB::rocksdb
                    gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++
                    # re2::re2 c-ares::cares)
                    Threads::Threads
                    )