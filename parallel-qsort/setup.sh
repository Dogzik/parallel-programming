#!/bin/bash

git clone https://github.com/google/googletest.git tests/lib
git clone https://github.com/google/benchmark.git benchmarks/lib
wget "https://github.com/OpenCilk/opencilk-project/releases/download/opencilk%2Fv1.0/docker-opencilk-v1.0.tar.gz"
docker load < docker-opencilk-v1.0.tar.gz
