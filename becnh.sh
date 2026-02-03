#!/bin/bash

run() {
    echo ""
    echo "Running $2 rows"
    time ./bin/main.cpp.out $1
    sleep 1
}   

run data/measurements.100.txt "100"
run data/measurements.1000.txt "1K"
run data/measurements.10000.txt "10K"
run data/measurements.100000.txt "100K"
run data/measurements.1000000.txt "1M"
run data/measurements.10000000.txt "10M"
run data/measurements.100000000.txt "100M"
run data/measurements.1000000000.txt "1B"
