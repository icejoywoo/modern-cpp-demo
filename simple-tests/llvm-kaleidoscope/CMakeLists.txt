option(BUILD_WITH_LLVM "build with contrib/llvm" OFF)

if (TARGET ch_contrib::llvm AND BUILD_WITH_LLVM)
    set(USE_EMBEDDED_COMPILER 1)
    message(STATUS "build with contrib/llvm")
endif()

if (USE_EMBEDDED_COMPILER)
    add_executable(toy toy.cpp)
    target_link_libraries(toy PRIVATE ch_contrib::llvm)
endif()
