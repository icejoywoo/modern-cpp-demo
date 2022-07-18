# just for tracer_test
add_library(tracer basics/tracer.cpp)
target_include_directories(tracer PUBLIC basics/tracer.hpp)

FILE(GLOB_RECURSE SRC_FILES "*_test.cc" "*_test.cpp" "*Test.cc" "*Test.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
    #    MESSAGE(STATUS "Found test file: ${FILE_PATH}")
    add_simple_gtest_file(${FILE_PATH} LIBRARY tracer)
ENDFOREACH(FILE_PATH)