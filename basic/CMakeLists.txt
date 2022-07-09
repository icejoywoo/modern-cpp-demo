add_executable(hello hello.cc)

add_executable(lambda_test lambda_test.cc)
target_link_libraries(lambda_test gtest gtest_main)
gtest_add_tests(TARGET lambda_test AUTO)