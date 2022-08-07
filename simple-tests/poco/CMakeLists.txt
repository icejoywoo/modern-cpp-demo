add_executable(poco-example example.cpp)
target_link_libraries(poco-example PRIVATE Poco::Foundation)

add_executable(http_server http_server.cpp)
target_link_libraries(http_server PRIVATE Poco::Foundation Poco::Net Poco::Util)