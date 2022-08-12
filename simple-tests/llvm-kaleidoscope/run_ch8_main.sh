#!/bin/bash

# $ ./toy
# ready> def average(x y) (x + y) * 0.5;
# ^D
# Wrote output.o

g++ ch8_main.cpp ../../_build/output.o -o main
./main
# average of 3.0 and 4.0: 3.5
