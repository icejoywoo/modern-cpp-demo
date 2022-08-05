FILE(GLOB_RECURSE SRC_FILES "*_demo.c" "*-demo.c" "*_demo.cpp" "*-demo.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
#    MESSAGE(STATUS "Found demo c++ file: ${FILE_PATH}")
    add_asan_demo_executable(${FILE_PATH})
ENDFOREACH(FILE_PATH)

add_executable(doctest_test doctest_test.cpp)
target_link_libraries(doctest_test doctest doctest_with_main)
add_test(doctest_test doctest_test)

add_executable(nanobench_test nanobench_test.cpp)
target_link_libraries(nanobench_test nanobench)

add_subdirectory(boost)
add_subdirectory(poco)

find_package(folly CONFIG)
if (folly_FOUND)
    find_package(gflags COMPONENTS shared)
    find_library(FMT fmt)
    find_library(EVENT event)
    find_library(DOUBLE_CONVERSION double-conversion)
    find_library(LZ4 lz4)
    find_library(LZO lzo2)
    find_library(ZSTD zstd)
    find_package(ZLIB)
    find_library(SNAPPY snappy)

    set(FOLLY_WITH_DEPENDENCIES
            ${FOLLY_LIBRARIES} ${Boost_LIBRARIES} ${DOUBLE_CONVERSION_LIBRARIES}
            ${EVENT} ${SNAPPY} ${CMAKE_DL_LIBS})

    set(FOLLY ${FOLLY_LIBRARIES})
    set(FOLLY_BENCHMARK Folly::follybenchmark)
    add_subdirectory(folly)
endif()