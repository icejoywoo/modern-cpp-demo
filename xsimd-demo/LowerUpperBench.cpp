#include "LowerUpperImpl.h"

#include "benchmark/benchmark.h"

#include <cstdint>
#include <random>
#include <vector>

void gen(char* res, size_t n) {
  static const char* dict = "AEDFGIJKLNOPQSUWXYZabcedefghijklmnopqrstuvwxyz";
  static const size_t dict_size = sizeof(dict) / sizeof(char);
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
  std::uniform_int_distribution<int> dist {0, dict_size};
  std::generate(res, res + n, [&dist, &mersenne_engine] {
    return dict[dist(mersenne_engine)];
  });
}

static void simd_array(benchmark::State& state) {
  // Perform setup here
  size_t size = state.range(0);
  char* src = new char[size];
  gen(src, size);
  char* dst = new char[size];
  for (auto _ : state) {
    DB::LowerUpperImpl<'A', 'Z'>::array(src, src + size, dst);
  }
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  benchmark::DoNotOptimize(src);
  benchmark::DoNotOptimize(dst);
  delete[] src;
  delete[] dst;
}

static void simple_array(benchmark::State& state) {
  // Perform setup here
  size_t size = state.range(0);
  char* src = new char[size];
  gen(src, size);
  char* dst = new char[size];
  for (auto _ : state) {
    DB::LowerUpperImpl<'A', 'Z'>::simple_array(src, src + size, dst);
  }
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  benchmark::DoNotOptimize(src);
  benchmark::DoNotOptimize(dst);
  delete[] src;
  delete[] dst;
}

// Register the function as a benchmark
BENCHMARK(simd_array)->Range(8, 8<<10);
BENCHMARK(simple_array)->Range(8, 8<<10);
// Run the benchmark
BENCHMARK_MAIN();