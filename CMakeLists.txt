cmake_minimum_required(VERSION 3.20)

project(modern-cpp-demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_program(CPPLINT_BIN
        NAMES cpplint cpplint.py
        HINTS "/usr/local/bin")
if (cpplint_FOUND)
    set(CMAKE_CXX_CPPLINT "cpplint;--exclude=third_party")
else()
    message(status "cpplint is not found. Please install cpplint to check code style.")
endif()

set(CMAKE_CXX_FLAGS "-Wall -g -O3 -fPIC")
set(CMAKE_C_FLAGS "-Wall -g -O3 -fPIC")

## google test
enable_testing()

add_subdirectory(basic)

add_subdirectory(third_party)