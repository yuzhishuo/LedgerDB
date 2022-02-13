project(Cli VERSION 0.1.0)


find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

# file(GLOB PROTO_FILES protos/*.proto)
# # need remove, so repeat define for each package, 
# set(SRCS "" CACHE FILEPATH "" FORCE )
# foreach(OBJ ${PROTO_FILES})
#     message(STATUS "Compiling ${OBJ}")
#     get_filename_component(OBJNAME ${OBJ} NAME_WLE)
#     set(proto_srcs ${CMAKE_CURRENT_BINARY_DIR}/protos/${OBJNAME}.pb.cc)
#     set(proto_hdrs ${CMAKE_CURRENT_BINARY_DIR}/protos/${OBJNAME}.pb.h)
#     list(APPEND SRCS ${proto_srcs})

#     set(grpc_srcs ${CMAKE_CURRENT_BINARY_DIR}/protos/${OBJNAME}.grpc.pb.cc)
#     set(grpc_hdrs ${CMAKE_CURRENT_BINARY_DIR}/protos/${OBJNAME}.grpc.pb.h)
#     list(APPEND SRCS ${grpc_srcs})
#     message(STATUS "Compiling ${_PROTOBUF_PROTOC}")
#     add_custom_command(
#       OUTPUT ${proto_srcs} ${proto_hdrs} ${grpc_srcs} ${grpc_hdrs}
#       COMMAND ${_PROTOBUF_PROTOC}
#       ARGS --grpc_out ${CMAKE_CURRENT_BINARY_DIR}/protos
#         --cpp_out ${CMAKE_CURRENT_BINARY_DIR}/protos
#         -I ${CMAKE_CURRENT_LIST_DIR}/protos
#         --plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE}
#         ${OBJ}
#       DEPENDS ${OBJ})
# endforeach()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(Cli main.cpp ${SRCS} )

target_link_libraries(Cli
    gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++
)