add_executable(hello hello.cc)

function(add_simple_gtest_file gtest_filename)
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" filename ${gtest_filename})
    add_executable(${filename} ${gtest_filename})
    target_link_libraries(${filename} gtest gtest_main)
    gtest_add_tests(TARGET ${filename} AUTO)
endfunction()

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