# protobuf demo
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS person.proto)
#message (STATUS "generated proto: ${PROTO_SRCS} ${PROTO_HDRS}")
add_library(_person_proto ${PROTO_SRCS})
target_link_libraries(_person_proto PUBLIC ch_contrib::protobuf)
target_include_directories(_person_proto SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
add_library(proto::person ALIAS _person_proto)

# grpc demo
PROTOBUF_GENERATE_GRPC_CPP(GRPC_SRCS GRPC_HDRS helloworld.proto)
#message (STATUS "generated grpc proto: ${GRPC_SRCS} ${GRPC_HDRS}")
add_library(_hello_proto ${GRPC_SRCS})
target_link_libraries(_hello_proto PUBLIC ch_contrib::grpc)
target_include_directories(_hello_proto SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

add_executable(greeter_client greeter_client.cpp)
target_link_libraries(greeter_client PRIVATE _hello_proto)

add_executable(greeter_server greeter_server.cpp)
target_link_libraries(greeter_server PRIVATE _hello_proto)

FILE(GLOB_RECURSE SRC_FILES "*_test.cc" "*_test.cpp" "*Test.cc" "*Test.cpp")
FOREACH(FILE_PATH ${SRC_FILES})
    add_simple_gtest_file(${FILE_PATH} LIBRARY proto::person)
ENDFOREACH(FILE_PATH)