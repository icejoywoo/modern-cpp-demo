add_executable(hello hello.cc)
target_link_libraries(hello glog gflags::gflags)

## google test
enable_testing()
include(GoogleTest) # gtest_add_tests / gtest_discover_tests

# google benchmark
add_executable(benchmark_demo benchmark_demo.cpp)
target_link_libraries(benchmark_demo benchmark::benchmark benchmark::benchmark_main pthread)

add_executable(align_demo align_demo.cpp)
target_link_libraries(align_demo benchmark::benchmark benchmark::benchmark_main pthread)

# https://www.cnblogs.com/tangxin-blog/p/7910699.html
FILE(GLOB_RECURSE SRC_FILES "*_test.cc" "*_test.cpp" "*Test.cc" "*Test.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
#    MESSAGE(STATUS "Found test file: ${FILE_PATH}")
    add_simple_gtest_file(${FILE_PATH})
ENDFOREACH(FILE_PATH)

# add gtest file example
# add_executable(lambda_test lambda_test.cpp)
# target_link_libraries(lambda_test gtest gtest_main)
# gtest_add_tests(TARGET lambda_test AUTO)