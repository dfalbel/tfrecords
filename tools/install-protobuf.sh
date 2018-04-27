#!/bin/sh

# install runtime and compiler on ubuntu for testing
curl -OL https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-cpp-3.5.1.zip
unzip protobuf-cpp-3.5.1.zip -d proto
cd proto/protobuf-3.5.1/
./configure
make
sudo make install
sudo ldconfig # refresh shared library cache.
