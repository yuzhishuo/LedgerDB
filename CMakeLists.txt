cmake_minimum_required(VERSION 3.14.0)
project(LedgerDB VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
file(COPY config.default.conf  DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
# memory leak detection
set(CMAKE_CXX_FLAGS "-fsanitize=address")
add_definitions(
  -DSPDLOG_FMT_EXTERNAL=ON
)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

find_package(RocksDB CONFIG REQUIRED)
find_package(Threads REQUIRED)

find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection )
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# raft 
set(RAFT_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/raft/libraft.a)
add_custom_target(build_raft ALL
        COMMAND ${CMAKE_MAKE_PROGRAM}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/raft
        COMMENT "Original raft makefile target")

add_library(raft STATIC IMPORTED)
set_property(TARGET raft APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(raft PROPERTIES IMPORTED_LOCATION_NOCONFIG ${RAFT_LIBRARY})

add_dependencies(raft build_raft)

include_directories(3rdParty/raft/include)
# raft end

add_subdirectory(3rdParty/googletest)
include_directories(3rdParty/googletest/googletest/include)
include_directories(3rdParty/googletest/googlemock/include)

add_subdirectory(3rdParty/fmt)
include_directories(3rdParty/fmt/include)

add_subdirectory(3rdParty/spdlog)
include_directories(3rdParty/spdlog/include)

add_subdirectory(protocol)
include_directories(protocol/gen)

include_directories(include)
add_subdirectory(include)

add_subdirectory(cli)

set(
  DIR_SRCS
  include/LedgerEngine.cpp
)

add_executable(Ledger ${DIR_SRCS} main.cpp )

# add_subdirectory(cli cli EXCLUDE_FROM_ALL)
# add_dependencies(Ledger Cli)
# add_dependencies(Ledger raft)

target_link_libraries(Ledger

                    RocksDB::rocksdb
                    gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++
                    # re2::re2 c-ares::cares
                    Threads::Threads
                    spdlog::spdlog spdlog::spdlog_header_only
                    rudeconfig
                    pthread rt
                    raft
                    )
target_link_libraries(Ledger
  merkle
  meta
  # rbac
  gen
  utility
  # raft_engine
)

enable_testing()
add_subdirectory(test)

add_test(NAME moontest COMMAND moontest) 
