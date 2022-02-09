FROM ubuntu:18.04

RUN apt update -y \
    && apt install -y cmake ccache libssl-dev libcrypto++-dev \
    libglib2.0-dev libltdl-dev libicu-dev libmysql++-dev \
    libreadline-dev libmysqlclient-dev unixodbc-dev \
    unixodbc-dev devscripts dupload fakeroot debhelper \
    gcc-7 g++-7 unixodbc-dev devscripts dupload fakeroot debhelper \
    liblld-5.0-dev libclang-5.0-dev liblld-5.0 \
    build-essential autoconf libtool pkg-config \
    libgflags-dev libgtest-dev \
    && apt-get update -y \
    && apt-get install -y gdb \
    && apt-get install libspdlog-dev \
    wget 

RUN cd /


RUN cd rocksdb && mkdir .build && cd .build &&  cmake .. -DCMAKE_BUILD_TYPE=Release && make -j $(nproc || grep -c ^processor /proc/cpuinfo) && make install -j $(nproc || grep -c ^processor /proc/cpuinfo)

RUN git clone --branch v1.9.2 https://gitee.com/y980620641/spdlog.git && cd spdlog && mkdir build && cd build \
    && cmake .. && make -j && make install