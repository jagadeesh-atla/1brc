#!/bin/bash

CFLAGS="-std=gnu17 -O2 "
CFLAGS+="-Wall -Wextra -Wconversion -Wformat -Wformat=2 -Wimplicit-fallthrough -Wvla"

gcc $CFLAGS -o bin/main.c.out c/main.c