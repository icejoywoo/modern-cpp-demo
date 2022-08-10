#include <benchmark/benchmark.h>

#include <iostream>
#include <memory>
#include <chrono>

static inline void *__movsb(void *d, const void *s, size_t n) {
#ifdef OS_APPLE_M1
    memcpy(d, s, n);
#else
    asm volatile ("rep movsb"
            : "=D" (d),
              "=S" (s),
              "=c" (n)
            : "0" (d),
              "1" (s),
              "2" (n)
            : "memory");
    return d;
#endif
}

/// result:
/// 1. 内存对齐：./align_demo
///     elapsed time = 1.32752
/// 2. 内存不对齐：./align_demo 1
///     elapsed time = 2.39101
static void BM_aligned(benchmark::State& state)
{
    constexpr int align_size = 32;
    constexpr int alloc_size = 10001;
    constexpr int buff_size = align_size + alloc_size;
    char dest[buff_size]{0};
    char src[buff_size]{0};
    void *dest_ori_ptr = dest;
    void *src_ori_ptr = src;
    size_t dest_size = sizeof(dest);
    size_t src_size = sizeof(src);
    char *dest_ptr = static_cast<char *>(std::align(align_size, alloc_size,  dest_ori_ptr, dest_size));
    char *src_ptr = static_cast<char *>(std::align(align_size, alloc_size,   src_ori_ptr, src_size));

    auto start = std::chrono::high_resolution_clock::now();
    for (auto _ : state) {
        __movsb(dest_ptr, src_ptr, alloc_size - 1);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
//    std::cout << "aligned elapsed time = " << diff.count() << "\n";
}

static void BM_non_aligned(benchmark::State& state)
{
    constexpr int align_size = 32;
    constexpr int alloc_size = 10001;
    constexpr int buff_size = align_size + alloc_size;
    char dest[buff_size]{0};
    char src[buff_size]{0};
    void *dest_ori_ptr = dest;
    void *src_ori_ptr = src;
    size_t dest_size = sizeof(dest);
    size_t src_size = sizeof(src);
    char *dest_ptr = static_cast<char *>(std::align(align_size, alloc_size,  dest_ori_ptr, dest_size));
    char *src_ptr = static_cast<char *>(std::align(align_size, alloc_size,   src_ori_ptr, src_size));

    // 测试内存不对齐的情况：传入参数的时候，将指针位置偏移，这样后续的内存都不对齐
    ++dest_ptr;
    ++src_ptr;

    auto start = std::chrono::high_resolution_clock::now();
    for (auto _ : state) {
        __movsb(dest_ptr, src_ptr, alloc_size - 1);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
//    std::cout << "non aligned elapsed time = " << diff.count() << "\n";
}

static int iterations = 10000000;
BENCHMARK(BM_aligned)->Iterations(iterations);
BENCHMARK(BM_non_aligned)->Iterations(iterations);

// Run the benchmark
BENCHMARK_MAIN();
