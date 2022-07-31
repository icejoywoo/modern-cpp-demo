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

// aligned vs unaligned, 差异不大
BENCHMARK_MAIN();