#include "simd_util.h"

#ifdef USE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif

#include "benchmark/benchmark.h"

static void BM_memset(benchmark::State& state) {
  // Perform setup here
  char* src = new char[state.range(0)];
  // change bytes value
  memset(src, simd::kNullByte, state.range(0));
  for (auto _ : state) {
    memset(src, simd::kNotNullByte, state.range(0));
  }
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  benchmark::DoNotOptimize(src);
  delete[] src;
}

static void BM_simd_memset(benchmark::State& state) {
  // Perform setup here
  char* src = new char[state.range(0)];
  // change bytes value
  memset(src, simd::kNullByte, state.range(0));
  for (auto _ : state) {
    simd::memset(src, simd::kNotNullByte, state.range(0));
  }
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  benchmark::DoNotOptimize(src);
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
  benchmark::DoNotOptimize(src);
  benchmark::DoNotOptimize(dst);
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
  benchmark::DoNotOptimize(src);
  benchmark::DoNotOptimize(dst);
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
BM_memset/8               3.61 ns         3.61 ns    208773113 bytes_per_second=2.06246G/s
BM_memset/64              2.06 ns         2.06 ns    297551527 bytes_per_second=28.9052G/s
BM_memset/512             6.14 ns         6.14 ns    112106388 bytes_per_second=77.6091G/s
BM_memset/4096            31.6 ns         31.6 ns     22361083 bytes_per_second=120.595G/s
BM_memset/8192            51.9 ns         51.9 ns     13182020 bytes_per_second=147.04G/s
BM_simd_memset/8          1.32 ns         1.32 ns    529820685 bytes_per_second=5.62938G/s
BM_simd_memset/64         1.16 ns         1.16 ns    607350113 bytes_per_second=51.5569G/s
BM_simd_memset/512        9.29 ns         9.29 ns     75694185 bytes_per_second=51.3083G/s
BM_simd_memset/4096       79.0 ns         79.0 ns      8902666 bytes_per_second=48.316G/s
BM_simd_memset/8192        153 ns          153 ns      4553673 bytes_per_second=49.7256G/s
BM_memcpy/8               2.60 ns         2.60 ns    269560785 bytes_per_second=2.87001G/s
BM_memcpy/64              2.31 ns         2.31 ns    303955742 bytes_per_second=25.8244G/s
BM_memcpy/512             6.32 ns         6.32 ns    119255584 bytes_per_second=75.4168G/s
BM_memcpy/4096            47.0 ns         47.0 ns     14882035 bytes_per_second=81.1167G/s
BM_memcpy/8192             147 ns          147 ns      4252338 bytes_per_second=51.765G/s
BM_simd_memcpy/8          1.44 ns         1.44 ns    483569016 bytes_per_second=5.16191G/s
BM_simd_memcpy/64         2.60 ns         2.60 ns    269567984 bytes_per_second=22.8841G/s
BM_simd_memcpy/512        12.7 ns         12.7 ns     55282551 bytes_per_second=37.452G/s
BM_simd_memcpy/4096        100 ns          100 ns      7010548 bytes_per_second=38.0612G/s
BM_simd_memcpy/8192        193 ns          193 ns      3629161 bytes_per_second=39.6195G/s
 */
BENCHMARK_MAIN();
