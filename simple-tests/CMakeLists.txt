FILE(GLOB_RECURSE SRC_FILES "*_demo.c" "*-demo.c" "*_demo.cpp" "*-demo.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
#    MESSAGE(STATUS "Found demo c++ file: ${FILE_PATH}")
    add_asan_demo_executable(${FILE_PATH})
ENDFOREACH(FILE_PATH)

add_executable(doctest_test doctest_test.cpp)
target_link_libraries(doctest_test doctest)
add_test(doctest_test doctest_test)

add_executable(nanobench_test nanobench_test.cpp)
target_link_libraries(nanobench_test nanobench)