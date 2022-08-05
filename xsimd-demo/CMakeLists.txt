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
# https://stackoverflow.com/questions/8843818/what-does-the-fpermissive-flag-do
# -fpermissive
# > Downgrade some diagnostics about nonconformant code from errors to warnings.
# > Thus, using -fpermissive will allow some nonconforming code to compile.
# Bottom line: don't use it unless you know what you are doing!
#target_compile_options(bit_packing_bench PRIVATE -fpermissive)

add_executable(restrict_bench restrict_bench.cpp)
target_link_libraries(restrict_bench benchmark::benchmark pthread)

add_executable(crc32_bench crc32_bench.cpp)
target_link_libraries(crc32_bench benchmark::benchmark pthread)