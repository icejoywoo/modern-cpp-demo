option(BUILD_WITH_LLVM "build with contrib/llvm" OFF)

if (TARGET ch_contrib::llvm AND BUILD_WITH_LLVM)
    set(USE_EMBEDDED_COMPILER 1)
    message(STATUS "build with contrib/llvm")
endif()

if (NOT USE_EMBEDDED_COMPILER)
    message(STATUS "Not build LLVM")
    return()
endif()

add_executable(toy toy.cpp)
target_link_libraries(toy PRIVATE ch_contrib::llvm)

add_executable(kaleidoscope_ch2 ch2.cpp)

add_executable(kaleidoscope_ch4 ch4.cpp)
target_link_libraries(kaleidoscope_ch4 PRIVATE ch_contrib::llvm)