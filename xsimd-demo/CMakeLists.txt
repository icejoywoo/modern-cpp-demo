FILE(GLOB_RECURSE SRC_FILES "*_test.cc" "*_test.cpp" "*Test.cc" "*Test.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
    add_simple_gtest_file(${FILE_PATH}
            LIBRARY
                xsimd
                simdjson::simdjson)
ENDFOREACH(FILE_PATH)

add_executable(xsimd_mem_bench xsimd_mem_bench.cpp)
target_link_libraries(xsimd_mem_bench benchmark::benchmark pthread xsimd)

add_executable(xsimd_aligned_bench xsimd_aligned_bench.cpp)
target_link_libraries(xsimd_aligned_bench benchmark::benchmark pthread xsimd)

add_executable(lower_upper_bench LowerUpperBench.cpp)
target_link_libraries(lower_upper_bench benchmark::benchmark pthread)

add_executable(bit_packing_bench bit_packing_bench.cpp)
target_link_libraries(bit_packing_bench benchmark::benchmark pthread)
target_compile_options(bit_packing_bench PRIVATE -fpermissive)