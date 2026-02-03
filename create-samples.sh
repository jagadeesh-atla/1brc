#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <size of data>"
    exit 1
fi

mkdir -p data bin

if [ -f "data/measurements.$1.txt" ]; then
    echo "File data/measurements.$1.txt already exists."
    exit 1
fi

gcc -lm create-samples.c -o bin/create-samples.out
./bin/create-samples.out $1 data/measurements.$1.txt
