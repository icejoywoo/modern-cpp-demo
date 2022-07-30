FILE(GLOB_RECURSE SRC_FILES "*_test.cc" "*_test.cpp" "*Test.cc" "*Test.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
    add_simple_gtest_file(${FILE_PATH}
            LIBRARY
                xsimd
                simdjson::simdjson)
ENDFOREACH(FILE_PATH)

add_executable(xsimd_mem_bench xsimd_mem_bench.cpp)
target_link_libraries(xsimd_mem_bench benchmark::benchmark pthread xsimd)