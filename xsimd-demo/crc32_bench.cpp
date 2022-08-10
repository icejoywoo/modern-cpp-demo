#include "benchmark/benchmark.h"

#ifdef __SSE__

#include <cstdint>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <smmintrin.h>

#define UInt64  uint64_t
#define StringKey8  UInt64

struct StringKey16
{
	UInt64 a;
	UInt64 b;
};

struct StringKey24
{
    UInt64 a;
    UInt64 b;
    UInt64 c;
};

struct StringRef
{
    const char * data;
    size_t size;
};

inline size_t hashKey8(StringKey8 key)
{
    size_t res = -1ULL;
    res = _mm_crc32_u64(res, key);
    return res;
}

inline size_t hashKey16(struct StringKey16 key)
{
    size_t res = -1ULL;
    res = _mm_crc32_u64(res, key.a);
    res = _mm_crc32_u64(res, key.b);
    return res;
}

inline size_t hashKey24(struct StringKey24 key)
{
    size_t res = -1ULL;
    res = _mm_crc32_u64(res, key.a);
    res = _mm_crc32_u64(res, key.b);
    res = _mm_crc32_u64(res, key.c);
    return res;
}

inline UInt64 unalignedLoad(const void * address)
{
    UInt64 res;
    memcpy(&res, address, sizeof(res));
    return res;
}

inline size_t stringRefHash(struct StringRef x)
{
    const char * pos = x.data;
    size_t size = x.size;

    if (size == 0)
        return 0;

    if (size < 8)
    {
        return 0;
    }

    const char * end = pos + size;
    size_t res = -1ULL;
    do
    {
        UInt64 word = unalignedLoad(pos);
        res = _mm_crc32_u64(res, word);

        pos += 8;
    } while (pos + 8 < end);

    UInt64 word = unalignedLoad(end - 8);    /// I'm not sure if this is normal.
    res = _mm_crc32_u64(res, word);

    return res;
}

static void BM_hashKey8(benchmark::State& state) {
  StringKey8 key8 = 1;
  size_t res = 0;
  for (auto _ : state) {
    res = hashKey8(key8);
  }
  benchmark::DoNotOptimize(res);
}

static void BM_hashKey16(benchmark::State& state) {
  struct StringKey16 key16;
  key16.a = 1;
  key16.b = 2;
  size_t res = 0;
  for (auto _ : state) {
    res = hashKey16(key16);
  }
  benchmark::DoNotOptimize(res);
}

static void BM_hashKey24(benchmark::State& state) {
  struct StringKey24 key24;
  key24.a = 1;
  key24.b = 2;
  key24.c = 3;
  size_t res = 0;
  for (auto _ : state) {
    res = hashKey24(key24);
  }
  benchmark::DoNotOptimize(res);
}

static void BM_stringRefHash(benchmark::State& state) {
  struct StringRef strRef;
  strRef.data = "123456789011121314151617181920";
  strRef.size = strlen(strRef.data);
  size_t res = 0;
  for (auto _ : state) {
    res = stringRefHash(strRef);
  }
  benchmark::DoNotOptimize(res);
}

BENCHMARK(BM_hashKey8)->Range(1024, 8<<12);
BENCHMARK(BM_hashKey16)->Range(1024, 8<<12);
BENCHMARK(BM_hashKey24)->Range(1024, 8<<12);
BENCHMARK(BM_stringRefHash)->Range(1024, 8<<12);

#endif

/**
 * result:
Run on (16 X 3500.33 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1024 KiB (x8)
  L3 Unified 36608 KiB (x1)
Load Average: 4.12, 3.07, 1.51
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
BM_hashKey8/1024            0.288 ns        0.288 ns   1000000000
BM_hashKey8/4096            0.288 ns        0.288 ns   1000000000
BM_hashKey8/32768           0.288 ns        0.288 ns   1000000000
BM_hashKey16/1024           0.288 ns        0.288 ns   1000000000
BM_hashKey16/4096           0.288 ns        0.288 ns   1000000000
BM_hashKey16/32768          0.288 ns        0.288 ns   1000000000
BM_hashKey24/1024           0.288 ns        0.288 ns   1000000000
BM_hashKey24/4096           0.288 ns        0.288 ns   1000000000
BM_hashKey24/32768          0.288 ns        0.288 ns   1000000000
BM_stringRefHash/1024       0.288 ns        0.288 ns   1000000000
BM_stringRefHash/4096       0.288 ns        0.288 ns   1000000000
BM_stringRefHash/32768      0.288 ns        0.288 ns   1000000000
 */
BENCHMARK_MAIN();