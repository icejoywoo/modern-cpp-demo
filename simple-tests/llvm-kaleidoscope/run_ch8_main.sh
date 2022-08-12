#!/bin/bash

# $ ./toy
# ready> def average(x y) (x + y) * 0.5;
# ^D
# Wrote output.o

BUILD_DIR=../../_build/

${BUILD_DIR}/simple-tests/llvm-kaleidoscope/kaleidoscope_ch8 < average.txt > /dev/null 2>&1

g++ ch8_main.cpp output.o -o main && ./main
# average of 3.0 and 4.0: 3.5

# clean

rm ./main output.o
