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
    for (int i = 0; i < n; i++) {
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

// Register the function as a benchmark
BENCHMARK(BM_unaligned)->Range(8, 8<<10);
BENCHMARK(BM_aligned)->Range(8, 8<<10);

/**
 * mac 上测试差异不大，在 linux 上测试差异很大，aligned 速度明显更快
 * result on ubuntu 20.04 (16 cores, 64GB):
Run on (16 X 3500.78 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1024 KiB (x8)
  L3 Unified 36608 KiB (x1)
Load Average: 5.17, 2.69, 5.01
----------------------------------------------------------------------------
Benchmark                  Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------
BM_unaligned/8          3.46 ns         3.46 ns    202340130 bytes_per_second=2.15412G/s
BM_unaligned/64         23.6 ns         23.6 ns     29598115 bytes_per_second=2.52133G/s
BM_unaligned/512         239 ns          239 ns      2949608 bytes_per_second=1.99378G/s
BM_unaligned/4096       2069 ns         2069 ns       337888 bytes_per_second=1.84364G/s
BM_unaligned/8192       4042 ns         4042 ns       174740 bytes_per_second=1.88762G/s
BM_aligned/8            2.59 ns         2.59 ns    269963641 bytes_per_second=2.87369G/s
BM_aligned/64           11.7 ns         11.7 ns     59941634 bytes_per_second=5.10527G/s
BM_aligned/512          91.8 ns         91.8 ns      7625224 bytes_per_second=5.19546G/s
BM_aligned/4096         1459 ns         1459 ns       479763 bytes_per_second=2.61505G/s
BM_aligned/8192         2934 ns         2934 ns       238683 bytes_per_second=2.60063G/s
 */
BENCHMARK_MAIN();