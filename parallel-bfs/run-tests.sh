#!/bin/bash

workers=${1:-4}
type=${2:-Release}

CILK_NWORKERS=$workers cmake-build-$type/run-tests
