FROM ubuntu:18.04

RUN apt update -y \
    && apt install -y cmake ccache libssl-dev libcrypto++-dev \
    libglib2.0-dev libltdl-dev libicu-dev libmysql++-dev \
    libreadline-dev libmysqlclient-dev unixodbc-dev \
    unixodbc-dev devscripts dupload fakeroot debhelper \
    gcc-7 g++-7 unixodbc-dev devscripts dupload fakeroot debhelper \
    liblld-5.0-dev libclang-5.0-dev liblld-5.0 \
    build-essential autoconf libtool pkg-config \
    libgflags-dev \
    && apt-get update -y \
    && apt-get install -y gdb \
    && apt-get install -y libspdlog-dev \
    wget tar

RUN cd /

RUN wget https://github.com/Kitware/CMake/releases/download/v3.19.2/cmake-3.19.2.tar.gz && tar -zxvf cmake-3.19.2.tar.gz \
    && cd cmake-3.19.2/  && ./bootstrap && make -j4 && make install


RUN git clone https://github.com/facebook/rocksdb.git && cd rocksdb && mkdir .build && cd .build &&  cmake .. -DCMAKE_BUILD_TYPE=Release && make -j $(nproc || grep -c ^processor /proc/cpuinfo) && make install -j $(nproc || grep -c ^processor /proc/cpuinfo)

RUN git clone --branch v1.9.2 https://github.com/gabime/spdlog.git && cd spdlog && mkdir build && cd build \
    && cmake .. && make -j && make install

RUN git clone https://github.com/google/googletest && cd googletest  && mkdir build && cd build && cmake .. && make -j $(nproc || grep -c ^processor /proc/cpuinfo) && make install

RUN git clone https://github.com/grpc/grpc.git && cd /grpc && git submodule update --init --recursive 


RUN cd /grpc && mkdir .build && cd .build && cmake .. -DgRPC_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release && make install -j $(nproc || grep -c ^processor /proc/cpuinfo) \
    && rm -rf /grpc/.build \
    && cd /grpc && mkdir .build && cd .build && cmake .. -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DgRPC_PROTOBUF_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package -DgRPC_CARES_PROVIDER=package -DgRPC_SSL_PROVIDER=package -DCMAKE_BUILD_TYPE=Release && make install -j $(nproc || grep -c ^processor /proc/cpuinfo)

# COPY *  /Ledger/

# RUN cd /Ledger/ && git submodule update --init --recursive  && mkdir .build && cd .build && cmake .. -DCMAKE_BUILD_TYPE=Release && make -j $(nproc || grep -c ^processor /proc/cpuinfo)