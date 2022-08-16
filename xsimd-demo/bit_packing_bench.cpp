#include "bit_packing.h"

#include "benchmark/benchmark.h"

#include <random>

using namespace bit::packing;

void gen(uint8_t* res, size_t n) {
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
  std::uniform_int_distribution<size_t> dist {0, 1};
  std::generate(res, res + n, [&dist, &mersenne_engine] {
    return dist(mersenne_engine);
  });
}

static void simple(benchmark::State& state) {
  size_t size = state.range(0);
  auto* input = new uint8_t[size];
  gen(input, size);
  size_t output_len = size / 8 + 1;
  auto* output = new uint8_t[output_len];
  for (auto _ : state) {
    Simple::encodeAsBits(input, size, output);
  }
  benchmark::DoNotOptimize(input);
  benchmark::DoNotOptimize(output);
  delete[] input;
  delete[] output;
}

static void multiplication(benchmark::State& state) {
  size_t size = state.range(0);
  auto* input = new uint8_t[size];
  gen(input, size);
  size_t output_len = size / 8 + 1;
  auto* output = new uint8_t[output_len];
  for (auto _ : state) {
    Multiplication::encodeAsBits(input, size, output);
  }
  benchmark::DoNotOptimize(input);
  benchmark::DoNotOptimize(output);
  delete[] input;
  delete[] output;
}

static void packed_struct(benchmark::State& state) {
  size_t size = state.range(0);
  auto* input = new uint8_t[size];
  gen(input, size);
  size_t output_len = size / 8 + 1;
  auto* output = new uint8_t[output_len];
  for (auto _ : state) {
    PackedStruct::encodeAsBits(input, size, output);
  }
  benchmark::DoNotOptimize(input);
  benchmark::DoNotOptimize(output);
  delete[] input;
  delete[] output;
}

static void bit_hacks(benchmark::State& state) {
    size_t size = state.range(0);
    auto* input = new uint8_t[size];
    gen(input, size);
    size_t output_len = size / 8 + 1;
    auto* output = new uint8_t[output_len];
    for (auto _ : state) {
        BitHacks::encodeAsBits(input, size, output);
    }
    benchmark::DoNotOptimize(input);
    benchmark::DoNotOptimize(output);
    delete[] input;
    delete[] output;
}

// Register the function as a benchmark
BENCHMARK(simple)->Range(8, 8<<12);
BENCHMARK(multiplication)->Range(8, 8<<12);
BENCHMARK(packed_struct)->Range(8, 8<<12);
BENCHMARK(bit_hacks)->Range(8, 8<<12);

#ifdef __SSE2__
static void sse2(benchmark::State& state) {
  size_t size = state.range(0);
  auto* input = new uint8_t[size];
  gen(input, size);
  size_t output_len = size / 8 + 1;
  auto* output = new uint8_t[output_len];
  for (auto _ : state) {
    Sse2::encodeAsBits(input, size, output);
  }
  benchmark::DoNotOptimize(input);
  benchmark::DoNotOptimize(output);
  delete[] input;
  delete[] output;
}

BENCHMARK(sse2)->Range(8, 8<<12);
#endif

// Run the benchmark
BENCHMARK_MAIN();