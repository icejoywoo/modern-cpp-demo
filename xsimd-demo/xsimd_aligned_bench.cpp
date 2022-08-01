#include "xsimd/xsimd.hpp"

#include "benchmark/benchmark.h"

#include <vector>

// https://xsimd.readthedocs.io/en/latest/vectorized_code.html
// xsimd::dispatch(mean{})(a, b, res, xsimd::aligned_mode / xsimd::unaligned_mode)
// arch-independent code
struct mean {
    template <class C, class Tag, class Arch>
    void operator()(Arch, const C& a, const C& b, C& res, Tag)
    {
        using b_type = xsimd::batch<double, Arch>;
        std::size_t inc = b_type::size;
        std::size_t size = res.size();
        // size for which the vectorization is possible
        std::size_t vec_size = size - size % inc;
        for(std::size_t i = 0; i < vec_size; i += inc)
        {
            b_type avec = b_type::load(&a[i], Tag());
            b_type bvec = b_type::load(&b[i], Tag());
            b_type rvec = (avec + bvec) / 2;
            xsimd::store(&res[i], rvec, Tag());
        }
        // Remaining part that cannot be vectorized
        for(std::size_t i = vec_size; i < size; ++i)
        {
            res[i] = (a[i] + b[i]) / 2;
        }
    }
};

template <typename T>
std::vector<T> gen(size_t n, T start, T delta) {
    std::vector<T> res;
    res.reserve(n);
    for (size_t i = 0; i < n; i++) {
        res.push_back(i + start + delta);
    }
    return res;
}

static void BM_unaligned(benchmark::State& state) {
    int64_t size = state.range(0);
    std::vector<double> a = gen(size, 1.0, 0.5);
    std::vector<double> b = gen(size, 2.0, 0.5);
    std::vector<double> res(a.size());

    for (auto _ : state) {
        xsimd::dispatch(mean{})(a, b, res, xsimd::unaligned_mode());
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(a.size()));
    benchmark::DoNotOptimize(res);
}

template <typename T>
struct aligned_type {
    typedef std::vector<T, xsimd::aligned_allocator<T>> vector;
};

static void BM_aligned(benchmark::State& state) {
    int64_t size = state.range(0);
    std::vector<double> tmp_a = gen(size, 1.0, 0.5);
    std::vector<double> tmp_b = gen(size, 2.0, 0.5);
    aligned_type<double>::vector a(tmp_a.begin(), tmp_a.end());
    aligned_type<double>::vector b(tmp_b.begin(), tmp_b.end());
    aligned_type<double>::vector res(a.size());

    for (auto _ : state) {
        xsimd::dispatch(mean{})(a, b, res, xsimd::aligned_mode());
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(a.size()));
    benchmark::DoNotOptimize(res);
}

static void BM_iterate_without_xsimd(benchmark::State& state) {
  int64_t size = state.range(0);
  std::vector<double> a = gen(size, 1.0, 0.5);
  std::vector<double> b = gen(size, 2.0, 0.5);
  std::vector<double> res(a.size());

  for (auto _ : state) {
    for(int64_t i = 0; i < size; ++i)
    {
      res[i] = (a[i] + b[i]) / 2;
    }
  }

  state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(a.size()));
  benchmark::DoNotOptimize(res);
}

// Register the function as a benchmark
BENCHMARK(BM_unaligned)->Range(8, 8<<10);
BENCHMARK(BM_aligned)->Range(8, 8<<10);
BENCHMARK(BM_iterate_without_xsimd)->Range(8, 8<<10);

/**
 * mac 上测试差异不大，在 linux 上测试差异很大，aligned 速度明显更快
 * result on ubuntu 20.04 (16 cores, 64GB):
Run on (16 X 3500.78 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1024 KiB (x8)
  L3 Unified 36608 KiB (x1)
Load Average: 6.00, 2.44, 1.18
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
BM_unaligned/8                      5.35 ns         5.28 ns    100000000 bytes_per_second=1.41103G/s
BM_unaligned/64                     35.4 ns         35.4 ns     19217576 bytes_per_second=1.68606G/s
BM_unaligned/512                     224 ns          224 ns      2538788 bytes_per_second=2.13288G/s
BM_unaligned/4096                   2100 ns         2100 ns       333004 bytes_per_second=1.81641G/s
BM_unaligned/8192                   4791 ns         4791 ns       173007 bytes_per_second=1.59257G/s
BM_aligned/8                        2.61 ns         2.61 ns    268265277 bytes_per_second=2.85433G/s
BM_aligned/64                       11.8 ns         11.8 ns     56701319 bytes_per_second=5.06946G/s
BM_aligned/512                       117 ns          117 ns      5968454 bytes_per_second=4.06746G/s
BM_aligned/4096                     1490 ns         1490 ns       468275 bytes_per_second=2.56087G/s
BM_aligned/8192                     2980 ns         2980 ns       234999 bytes_per_second=2.56055G/s
BM_iterate_without_xsimd/8          2.88 ns         2.88 ns    241544627 bytes_per_second=2.58537G/s
BM_iterate_without_xsimd/64         17.2 ns         17.2 ns     40857431 bytes_per_second=3.46354G/s
BM_iterate_without_xsimd/512         166 ns          166 ns      4232911 bytes_per_second=2.86888G/s
BM_iterate_without_xsimd/4096       1826 ns         1826 ns       382811 bytes_per_second=2.08962G/s
BM_iterate_without_xsimd/8192       3652 ns         3652 ns       191556 bytes_per_second=2.08897G/s
 */
BENCHMARK_MAIN();