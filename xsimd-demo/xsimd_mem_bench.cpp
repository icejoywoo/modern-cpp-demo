#include "simd_util.h"

#include "benchmark/benchmark.h"

static void BM_memset(benchmark::State& state) {
  // Perform setup here
  char* src = new char[state.range(0)];
  for (auto _ : state) {
    memset(src, simd::kNotNullByte, state.range(0));
  }
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
}

static void BM_simd_memset(benchmark::State& state) {
  // Perform setup here
  char* src = new char[state.range(0)];
  for (auto _ : state) {
    simd::memset(src, simd::kNotNullByte, state.range(0));
  }
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
}

// example from https://github.com/google/benchmark/blob/main/docs/user_guide.md
static void BM_memcpy(benchmark::State& state) {
  char* src = new char[state.range(0)];
  char* dst = new char[state.range(0)];
  memset(src, simd::kNotNullByte, state.range(0));
  for (auto _ : state) {
    memcpy(dst, src, state.range(0));
  }
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
  delete[] dst;
}

static void BM_simd_memcpy(benchmark::State& state) {
  char* src = new char[state.range(0)];
  char* dst = new char[state.range(0)];
  simd::memset(src, simd::kNotNullByte, state.range(0));
  for (auto _ : state) {
    simd::memcpy(dst, src, state.range(0));
  }
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
  delete[] dst;
}

// Register the function as a benchmark
BENCHMARK(BM_memset)->Range(8, 8<<10);
BENCHMARK(BM_simd_memset)->Range(8, 8<<10);
BENCHMARK(BM_memcpy)->Range(8, 8<<10);
BENCHMARK(BM_simd_memcpy)->Range(8, 8<<10);

/**
 * Run on (16 X 3386.44 MHz CPU s)
 * CPU Caches:
 *   L1 Data 32 KiB (x8)
 *   L1 Instruction 32 KiB (x8)
 *   L2 Unified 1024 KiB (x8)
 *   L3 Unified 36608 KiB (x1)
 * Load Average: 4.08, 2.57, 2.15
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
BM_memset/8               3.72 ns         3.72 ns    253067107 bytes_per_second=2.003G/s
BM_memset/64              2.34 ns         2.34 ns    226534778 bytes_per_second=25.4281G/s
BM_memset/512             6.07 ns         6.07 ns    115743066 bytes_per_second=78.5751G/s
BM_memset/4096            31.9 ns         31.9 ns     22017829 bytes_per_second=119.732G/s
BM_memset/8192            50.1 ns         50.1 ns     13280611 bytes_per_second=152.155G/s
BM_simd_memset/8         0.906 ns        0.906 ns    772032339 bytes_per_second=8.2201G/s
BM_simd_memset/64         1.16 ns         1.16 ns    606626765 bytes_per_second=51.283G/s
BM_simd_memset/512        9.25 ns         9.25 ns     75203966 bytes_per_second=51.5341G/s
BM_simd_memset/4096       74.3 ns         74.3 ns      9453871 bytes_per_second=51.3376G/s
BM_simd_memset/8192        148 ns          148 ns      4727068 bytes_per_second=51.5453G/s
BM_memcpy/8               2.32 ns         2.32 ns    303156115 bytes_per_second=3.21661G/s
BM_memcpy/64              2.02 ns         2.02 ns    345579605 bytes_per_second=29.4923G/s
BM_memcpy/512             6.30 ns         6.30 ns    121482975 bytes_per_second=75.6569G/s
BM_memcpy/4096            47.0 ns         47.0 ns     14899808 bytes_per_second=81.2323G/s
BM_memcpy/8192             153 ns          153 ns      5298003 bytes_per_second=49.9432G/s
BM_simd_memcpy/8          2.09 ns         2.09 ns    334737761 bytes_per_second=3.56888G/s
BM_simd_memcpy/64         2.60 ns         2.60 ns    269605956 bytes_per_second=22.9635G/s
BM_simd_memcpy/512        13.0 ns         13.0 ns     55024367 bytes_per_second=36.7717G/s
BM_simd_memcpy/4096        104 ns          104 ns      7129309 bytes_per_second=36.561G/s
BM_simd_memcpy/8192        197 ns          197 ns      3563015 bytes_per_second=38.8089G/s
 */
BENCHMARK_MAIN();