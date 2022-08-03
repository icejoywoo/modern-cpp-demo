FILE(GLOB_RECURSE SRC_FILES "*_test.cc" "*_test.cpp" "*Test.cc" "*Test.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
    #    MESSAGE(STATUS "Found test file: ${FILE_PATH}")
    add_simple_gtest_file(${FILE_PATH} LIBRARY Folly::folly gflags::gflags Folly::follybenchmark)
ENDFOREACH(FILE_PATH)

add_executable(f14_bench f14_bench.cpp)
target_link_libraries(f14_bench PRIVATE Folly::folly Folly::follybenchmark)

add_executable(json_bench json_bench.cpp)
target_link_libraries(json_bench PRIVATE Folly::folly Folly::follybenchmark simdjson::simdjson)