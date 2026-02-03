#!/bin/bash

CFLAGS="-std=c++23 -O3 -g -pthread "
CFLAGS+="-Wall -Wextra -Wconversion -Wformat -Wformat=2 -Wimplicit-fallthrough -Wvla"

g++ $CFLAGS -o bin/main.cpp.out cpp/main.cpp