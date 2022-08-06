FROM ubuntu:18.04

# for gcc-10
RUN apt update -y
RUN apt install -y  software-properties-common dirmngr apt-transport-https lsb-release ca-certificates
RUN add-apt-repository ppa:ubuntu-toolchain-r/test

RUN apt update -y \
    && apt install -y cmake ccache libssl-dev libcrypto++-dev \
    libglib2.0-dev libltdl-dev libicu-dev libmysql++-dev \
    libreadline-dev libmysqlclient-dev unixodbc-dev \
    unixodbc-dev devscripts dupload fakeroot debhelper \
    gcc-10 g++-10 unixodbc-dev devscripts dupload fakeroot debhelper \
    liblld-5.0-dev libclang-5.0-dev liblld-5.0 \
    build-essential autoconf libtool pkg-config \
    libgflags-dev \
    && apt-get update -y \
    && apt-get install -y gdb clang-format \
    wget tar

RUN cd /

# lib
RUN  apt-get -y install flex bison libudev-dev 

RUN wget http://rudeserver.com/config/download/rudeconfig-5.0.5.tar.gz && tar -zxvf rudeconfig-5.0.5.tar.gz && cd rudeconfig-5.0.5 && ./configure && make && make install

RUN wget https://github.com/Kitware/CMake/releases/download/v3.19.2/cmake-3.19.2.tar.gz && tar -zxvf cmake-3.19.2.tar.gz \
    && cd cmake-3.19.2/  && ./bootstrap && make -j4 && make install

RUN git clone -b v1.47.x https://github.com/grpc/grpc.git && cd /grpc && git submodule update --init --recursive 

RUN cd /grpc && mkdir .build && cd .build && cmake .. -DgRPC_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release && make install -j $(nproc || grep -c ^processor /proc/cpuinfo) \
    && rm -rf /grpc/.build \
    && cd /grpc && mkdir .build && cd .build && cmake .. -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DgRPC_PROTOBUF_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package -DgRPC_CARES_PROVIDER=package -DgRPC_SSL_PROVIDER=package -DCMAKE_BUILD_TYPE=Release && make install -j $(nproc || grep -c ^processor /proc/cpuinfo)