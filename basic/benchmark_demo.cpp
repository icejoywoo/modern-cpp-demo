#include <benchmark/benchmark.h>

#include <cstring>
#include <memory>

// https://github.com/google/benchmark/issues/1217
// It does not work!
class CustomMemoryManager: public benchmark::MemoryManager {
 public:

  int64_t num_allocs;
  int64_t max_bytes_used;


  void Start() BENCHMARK_OVERRIDE {
    num_allocs = 0;
    max_bytes_used = 0;
  }

  void Stop(Result* result) BENCHMARK_OVERRIDE {
    result->num_allocs = num_allocs;
    result->max_bytes_used = max_bytes_used;
  }
};

std::unique_ptr<CustomMemoryManager> mm(new CustomMemoryManager());

//#ifdef MEMORY_PROFILER
void *custom_malloc(size_t size) {
  void *p = malloc(size);
  mm.get()->num_allocs += 1;
  mm.get()->max_bytes_used += size;
  return p;
}
#define malloc(size) custom_malloc(size)
//#endif

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
    benchmark::DoNotOptimize(src);
    benchmark::DoNotOptimize(dst);
    delete[] src;
    delete[] dst;
}

// Register the function as a benchmark
BENCHMARK(BM_memcpy)->Range(8, 8<<10);
// Run the benchmark
// BENCHMARK_MAIN();
// args: --benchmark_format=json
//
int main(int argc, char** argv)
{
  ::benchmark::RegisterMemoryManager(mm.get());
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::RegisterMemoryManager(nullptr);
}