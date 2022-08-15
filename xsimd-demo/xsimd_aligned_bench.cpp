#include "xsimd/xsimd.hpp"

#include "benchmark/benchmark.h"

#include <vector>

#if XSIMD_WITH_AVX || XSIMD_WITH_SSE2 || XSIMD_WITH_AVX512F || XSIMD_WITH_NEON

// https://xsimd.readthedocs.io/en/latest/vectorized_code.html
// xsimd::dispatch(mean{})(a, b, res, xsimd::aligned_mode / xsimd::unaligned_mode)
// arch-independent code
struct mean {
    template <class C, class Tag, class Arch>
    void operator()(Arch, const C& a, const C& b, C& res, Tag)
    {
#if XSIMD_WITH_AVX || XSIMD_WITH_SSE2 || XSIMD_WITH_AVX512F
        using b_type = xsimd::batch<double, Arch>;
#endif
#if XSIMD_WITH_NEON
        using b_type = xsimd::batch<float, Arch>;
#endif
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

#endif

template <typename T>
std::vector<T> gen(size_t n, T start, T delta) {
    std::vector<T> res;
    res.reserve(n);
    for (size_t i = 0; i < n; i++) {
        res.push_back(i + start + delta);
    }
    return res;
}

#if XSIMD_WITH_AVX || XSIMD_WITH_SSE2 || XSIMD_WITH_AVX512F

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
#endif

#if XSIMD_WITH_NEON

static void BM_unaligned(benchmark::State& state) {
  int64_t size = state.range(0);
  std::vector<float> a = gen(size, 1.0f, 0.5f);
  std::vector<float> b = gen(size, 2.0f, 0.5f);
  std::vector<float> res(a.size());

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
  std::vector<float> tmp_a = gen(size, 1.0f, 0.5f);
  std::vector<float> tmp_b = gen(size, 2.0f, 0.5f);
  aligned_type<float>::vector a(tmp_a.begin(), tmp_a.end());
  aligned_type<float>::vector b(tmp_b.begin(), tmp_b.end());
  aligned_type<float>::vector res(a.size());

  for (auto _ : state) {
    xsimd::dispatch(mean{})(a, b, res, xsimd::aligned_mode());
  }

  state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(a.size()));
  benchmark::DoNotOptimize(res);
}
#endif

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
#if XSIMD_WITH_AVX || XSIMD_WITH_SSE2 || XSIMD_WITH_AVX512F || XSIMD_WITH_NEON
BENCHMARK(BM_unaligned)->Range(8, 8<<10);
BENCHMARK(BM_aligned)->Range(8, 8<<10);
#endif
BENCHMARK(BM_iterate_without_xsimd)->Range(8, 8<<10);

/**
 * mac 上测试差异不大，在 linux 上测试差异很大，aligned 速度明显更快
 * 1. result on ubuntu 20.04 (16 cores, 64GB):
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

 * 2. result on M1 Pro: m1 上 aligned 和 unaligned 性能差异不太大，并且 neon 好像不支持 double 类型，所以改为 float 类型
Run on (10 X 24.0242 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 10.03, 7.14, 5.56
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
BM_unaligned/8                      1.69 ns         1.69 ns    411575865 bytes_per_second=4.40027G/s
BM_unaligned/64                     10.1 ns         10.0 ns     71493499 bytes_per_second=5.93326G/s
BM_unaligned/512                    83.1 ns         83.0 ns      8516955 bytes_per_second=5.7421G/s
BM_unaligned/4096                    642 ns          641 ns      1082988 bytes_per_second=5.95137G/s
BM_unaligned/8192                   1272 ns         1272 ns       556921 bytes_per_second=5.99962G/s
BM_aligned/8                        1.72 ns         1.72 ns    419448006 bytes_per_second=4.33594G/s
BM_aligned/64                       10.2 ns         10.2 ns     69532049 bytes_per_second=5.8215G/s
BM_aligned/512                      84.1 ns         84.1 ns      8416598 bytes_per_second=5.67155G/s
BM_aligned/4096                      628 ns          628 ns      1092999 bytes_per_second=6.07451G/s
BM_aligned/8192                     1282 ns         1278 ns       543356 bytes_per_second=5.97115G/s
BM_iterate_without_xsimd/8          2.37 ns         2.35 ns    296953679 bytes_per_second=3.16671G/s
BM_iterate_without_xsimd/64         9.39 ns         9.38 ns     75179087 bytes_per_second=6.3517G/s
BM_iterate_without_xsimd/512        71.8 ns         71.8 ns      9674120 bytes_per_second=6.64135G/s
BM_iterate_without_xsimd/4096        527 ns          527 ns      1318913 bytes_per_second=7.24357G/s
BM_iterate_without_xsimd/8192       1040 ns         1040 ns       685851 bytes_per_second=7.33684G/s
 */
BENCHMARK_MAIN();