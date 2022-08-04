#include "benchmark/benchmark.h"

int add1(int* a, int* b)
{
  *a = 10;
  *b = 12;
  return *a + *b;
}

int add2(int* __restrict  a, int* __restrict b)
{
  *a = 10;
  *b = 12;
  return *a + *b ;
}

static void simple(benchmark::State& state) {
  int result = 0;
  int * a = new int;
  int * b = new int;
  for (auto _ : state) {
    result = add1(a, b);
  }
  benchmark::DoNotOptimize(result);
}

static void restrict(benchmark::State& state) {
  int result = 0;
  int * a = new int;
  int * b = new int;
  for (auto _ : state) {
    result = add2(a, b);
  }
  benchmark::DoNotOptimize(result);
}

BENCHMARK(simple)->Range(8, 8<<12);
BENCHMARK(restrict)->Range(8, 8<<12);

/**
 * https://llvm.org/devmtg/2017-02-04/Restrict-Qualified-Pointers-in-LLVM.pdf
 * example from https://zhuanlan.zhihu.com/p/349726808
---------------------------------------------------------
Benchmark               Time             CPU   Iterations
---------------------------------------------------------
simple/8             1.45 ns         1.45 ns    478998840
simple/64            1.45 ns         1.45 ns    483420006
simple/512           1.48 ns         1.48 ns    481732822
simple/4096          1.46 ns         1.46 ns    477895954
simple/32768         1.45 ns         1.45 ns    485307116
restrict/8           1.44 ns         1.44 ns    483567168
restrict/64          1.44 ns         1.44 ns    486854247
restrict/512         1.44 ns         1.44 ns    483643104
restrict/4096        1.44 ns         1.44 ns    482953817
restrict/32768       1.44 ns         1.44 ns    483386540
 */
BENCHMARK_MAIN();
