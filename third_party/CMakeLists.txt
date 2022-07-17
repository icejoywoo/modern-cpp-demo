
add_subdirectory(googletest)

# disable benchmark unittest
set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(benchmark)

# new unit test framework
add_subdirectory(doctest)
# new benchmark framework
add_subdirectory(nanobench)

add_subdirectory(xsimd)