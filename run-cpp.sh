#!/bin/bash

CFLAGS="-std=c++23 -O3 -g -pthread "
CFLAGS+="-Wall -Wextra -Wconversion -Wformat -Wformat=2 -Wimplicit-fallthrough -Wvla"

set -x

g++ $CFLAGS -o bin/main.cpp.out cpp/main.cpp
time bin/main.cpp.out data/measurements.1000000000.txt 