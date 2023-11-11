#!/usr/bin/bash
cmake -B ./build
cd build 
make all 
./tests
