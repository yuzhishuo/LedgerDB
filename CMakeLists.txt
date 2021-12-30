cmake_minimum_required(VERSION 3.0.0)
project(samplate-cpp-template VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(RocksDB CONFIG REQUIRED)

enable_testing()
include_directories(./include)
aux_source_directory(./src DIR_SRCS)
add_executable(samplate-cpp-template ${DIR_SRCS} main.cpp )

target_link_libraries(samplate-cpp-template casbin RocksDB::rocksdb)