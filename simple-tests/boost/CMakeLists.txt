FILE(GLOB_RECURSE SRC_FILES "*_test.cc" "*_test.cpp" "*Test.cc" "*Test.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
    #    MESSAGE(STATUS "Found test file: ${FILE_PATH}")
    add_simple_gtest_file(${FILE_PATH} LIBRARY Boost::boost Boost::filesystem)
ENDFOREACH(FILE_PATH)

add_executable(boost_filesystem_example boost_filesystem_example.cpp)
target_link_libraries(boost_filesystem_example PUBLIC Boost::boost Boost::filesystem)
