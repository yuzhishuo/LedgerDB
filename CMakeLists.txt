cmake_minimum_required(VERSION 3.0.0)
project(example_casbin_cpp VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

find_package(RocksDB CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(spdlog CONFIG REQUIRED)

find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection )
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)


message(STATUS "Using protobuf ${Protobuf_VERSION}")
message(STATUS "Using gRPC ${gRPC_VERSION}")

file(GLOB PROTO_FILES protos/*.proto)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/protos)

set(_GRPC_GRPCPP gRPC::grpc++ )
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin> )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LOCAL_W_FLAGS} ${LOCAL_C_FLAGS} -fPIC")

set(SRCS "" CACHE FILEPATH "" FORCE )
foreach(OBJ ${PROTO_FILES})
    message(STATUS "Compiling ${OBJ}")
    get_filename_component(OBJNAME ${OBJ} NAME_WLE)
    set(proto_srcs ${CMAKE_CURRENT_BINARY_DIR}/protos/${OBJNAME}.pb.cc)
    set(proto_hdrs ${CMAKE_CURRENT_BINARY_DIR}/protos/${OBJNAME}.pb.h)
    list(APPEND SRCS ${proto_srcs})

    set(grpc_srcs ${CMAKE_CURRENT_BINARY_DIR}/protos/${OBJNAME}.grpc.pb.cc)
    set(grpc_hdrs ${CMAKE_CURRENT_BINARY_DIR}/protos/${OBJNAME}.grpc.pb.h)
    list(APPEND SRCS ${grpc_srcs})
    message(STATUS "Compiling ${_PROTOBUF_PROTOC}")
    add_custom_command(
      OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}/protos"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/protos"
        -I "${CMAKE_CURRENT_LIST_DIR}/protos"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${OBJ}"
      DEPENDS "${OBJ}")
endforeach()

# raft 
set(RAFT_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/raft/libraft.a)
add_custom_target(build_raft ALL
        COMMAND ${CMAKE_MAKE_PROGRAM}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/raft
        COMMENT "Original raft makefile target")

        message(status "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/raft/libraft.a")
add_library(raft STATIC IMPORTED)
set_property(TARGET raft APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(raft PROPERTIES IMPORTED_LOCATION_NOCONFIG ${RAFT_LIBRARY})

add_dependencies(raft build_raft)

include_directories(3rdParty/raft/include)

# raft end

add_subdirectory(3rdParty/muduo)
include_directories(3rdParty/muduo)
include_directories(build/protos)
include_directories(include)

aux_source_directory(src DIR_SRCS)
add_executable(samplate-cpp-template ${DIR_SRCS} ${SRCS} main.cpp )

add_dependencies(samplate-cpp-template raft)
target_link_libraries(samplate-cpp-template

                    RocksDB::rocksdb
                    gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++
                    # re2::re2 c-ares::cares)
                    Threads::Threads
                    spdlog::spdlog spdlog::spdlog_header_only
                    rudeconfig
                    )
target_link_libraries(samplate-cpp-template muduo_net)
target_link_libraries(samplate-cpp-template muduo_base)
target_link_libraries(samplate-cpp-template pthread rt)
target_link_libraries(samplate-cpp-template raft)
# gcov: https://www.cnblogs.com/turtle-fly/archive/2013/01/06/2847190.html
# how to use : https://blog.csdn.net/yanxiangyfg/article/details/80989680
target_link_libraries(samplate-cpp-template gcov)
option(ENABLE_TEST "Enable test" ON)
if(${ENABLE_TEST})
    add_subdirectory(test)
endif()