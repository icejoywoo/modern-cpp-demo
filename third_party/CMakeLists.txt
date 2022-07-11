
add_subdirectory(googletest)

# disable benchmark unittest
set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(benchmark)

add_subdirectory(xsimd)