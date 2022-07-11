cmake_minimum_required(VERSION 3.20)

project(modern-cpp-demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

## google test
enable_testing()

add_subdirectory(basic)

add_subdirectory(third_party)