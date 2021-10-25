#!/bin/bash

type=${1:-Release}

mkdir -p cmake-build-$type
cd cmake-build-$type
cmake -DCMAKE_BUILD_TYPE=$type -DCMAKE_C_COMPILER=/usr/local/bin/clang -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ ..
cd ..
cmake --build cmake-build-$type
