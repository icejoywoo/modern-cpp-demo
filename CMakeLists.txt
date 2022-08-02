cmake_minimum_required(VERSION 3.5)

project(modern-cpp-demo C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
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

# refer https://github.com/facebookincubator/velox/blob/main/CMakeLists.txt
execute_process(
        COMMAND
        bash -c
        "( source ${CMAKE_CURRENT_SOURCE_DIR}/scripts/setup-helper-functions.sh && echo -n $(get_cxx_flags $ENV{CPU_TARGET}))"
        OUTPUT_VARIABLE SCRIPT_CXX_FLAGS
        RESULT_VARIABLE COMMAND_STATUS)

if(COMMAND_STATUS EQUAL "1")
    message(FATAL_ERROR "Unable to determine compiler flags!")
endif()
message("Setting CMAKE_CXX_FLAGS=${SCRIPT_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SCRIPT_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fPIC ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -fPIC ")

option(RELEASE_ENABLE_O3 "Build with -O3" OFF)

# debug with memory sanitizer
#if (CMAKE_BUILD_TYPE MATCHES Debug)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -fsanitize=address -fno-omit-frame-pointer ")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1 -fsanitize=address -fno-omit-frame-pointer ")
#else()
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
#endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -fsanitize=address -fno-omit-frame-pointer")

if (RELEASE_ENABLE_O3)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()

include_directories(.)
include_directories(third_party)

## google test
enable_testing()

add_subdirectory(basic)
add_subdirectory(template)
add_subdirectory(simple-tests)
add_subdirectory(xsimd-demo)

add_subdirectory(third_party)
