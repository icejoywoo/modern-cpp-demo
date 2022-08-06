PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS person.proto)

message (STATUS "generated proto: ${PROTO_SRCS} ${PROTO_HDRS}")
add_library(protos ${PROTO_SRCS})
target_link_libraries(protos PUBLIC ch_contrib::protobuf)
target_include_directories(protos SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB_RECURSE SRC_FILES "*_test.cc" "*_test.cpp" "*Test.cc" "*Test.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
    #    MESSAGE(STATUS "Found test file: ${FILE_PATH}")
    add_simple_gtest_file(${FILE_PATH} LIBRARY protos)
ENDFOREACH(FILE_PATH)