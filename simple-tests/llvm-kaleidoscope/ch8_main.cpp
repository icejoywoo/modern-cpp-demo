/**
$ ./simple-tests/llvm-kaleidoscope/kaleidoscope_ch8
ready> def average(x y) (x + y) * 0.5;
^D
Wrote output.o 

* ^D is ctrl+d

* clang++ ch8_main.cpp output.o -o main
 */
#include <iostream>

extern "C" {
    double average(double, double);
}

int main() {
    std::cout << "average of 3.0 and 4.0: " << average(3.0, 4.0) << std::endl;
}
