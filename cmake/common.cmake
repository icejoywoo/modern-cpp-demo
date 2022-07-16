# simple demo for memory sanitizer
function(add_asan_demo_executable filename_with_postfix)
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" filename ${filename_with_postfix})
    add_executable(${filename} ${filename_with_postfix})
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        target_compile_options(${filename} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    endif()
endfunction()

# add simple gtest file without third-party dependency
function(add_simple_gtest_file gtest_filename)
    set (options "")
    set (oneValueArgs LIBRARY)
    set (multiValueArgs "")
    CMAKE_PARSE_ARGUMENTS(GTEST
            "${options}"
            "${oneValueArgs}"
            "${multiValueArgs}"
            ${ARGN}
            )
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" filename ${gtest_filename})
    add_executable(${filename} ${gtest_filename})
    target_link_libraries(${filename} gtest gtest_main ${GTEST_LIBRARY})
    gtest_add_tests(TARGET ${filename} AUTO)
endfunction()