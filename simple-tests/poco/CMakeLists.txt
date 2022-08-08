add_executable(poco-example example.cpp)
target_link_libraries(poco-example PRIVATE Poco::Foundation)

add_executable(http_server http_server.cpp)
target_link_libraries(http_server PRIVATE Poco::Foundation Poco::Net Poco::Util)

FILE(GLOB_RECURSE SRC_FILES "*_test.cc" "*_test.cpp" "*Test.cc" "*Test.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
    add_simple_gtest_file(${FILE_PATH} LIBRARY Poco::Foundation)
ENDFOREACH(FILE_PATH)