cmake_minimum_required(VERSION 3.20)

project(modern-cpp-demo C CXX ASM)

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

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(cmake/common.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fPIC ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -fPIC ")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -fsanitize=address -fno-omit-frame-pointer ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1 -fsanitize=address -fno-omit-frame-pointer ")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()

## google test
enable_testing()

add_subdirectory(basic)
add_subdirectory(error-tests)

add_subdirectory(third_party)