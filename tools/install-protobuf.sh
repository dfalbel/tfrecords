#!/bin/sh

# install compiler
# curl -OL https://github.com/google/protobuf/releases/download/v3.5.1/protoc-3.5.1-linux-x86_64.zip
# unzip protoc-3.5.1-linux-x86_64.zip -d protoc3
# mv protoc3/bin/* /usr/local/bin/
# mv protoc3/include/* /usr/local/include/

# install runtime
curl -OL https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-cpp-3.5.1.zip
unzip protobuf-cpp-3.5.1.zip -d proto-run3
cd proto-run3/protobuf-3.5.1/
./configure
make
make check
sudo make install
sudo ldconfig # refresh shared library cache.
