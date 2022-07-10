#include <benchmark/benchmark.h>

// https://github.com/google/benchmark/blob/main/docs/user_guide.md
static void BM_memcpy(benchmark::State& state) {
    // Perform setup here
    char* src = new char[state.range(0)];
    char* dst = new char[state.range(0)];
    memset(src, 'x', state.range(0));
    for (auto _ : state) {
        memcpy(dst, src, state.range(0));
    }
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
    delete[] src;
    delete[] dst;
}
// Register the function as a benchmark
BENCHMARK(BM_memcpy)->Range(8, 8<<10);
// Run the benchmark
BENCHMARK_MAIN();