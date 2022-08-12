option(BUILD_WITH_LLVM "build with contrib/llvm" OFF)

if (TARGET ch_contrib::llvm AND BUILD_WITH_LLVM)
    set(USE_EMBEDDED_COMPILER 1)
    message(STATUS "build with contrib/llvm")
endif()

if (NOT USE_EMBEDDED_COMPILER)
    message(STATUS "Not build LLVM")
    return()
endif()

# refer ClickHouse src/CMakeLists.txt dbms_target_link_libraries
# https://cmake.org/cmake/help/latest/command/macro.html
macro (llvm_target_link_libraries)
    foreach (module ${all_modules})
        target_link_libraries (${module} ${ARGN})
    endforeach ()
endmacro ()

# https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html
add_executable(kaleidoscope_ch2 ch2.cpp)

set(LLVM_WITH_OrcJIT ch_contrib::llvm LLVMOrcJIT ${CMAKE_DL_LIBS} LLVMAArch64CodeGen LLVMX86CodeGen)

macro (add_kaleidoscope_target ch)
    list (APPEND all_modules kaleidoscope_${ch})
    add_executable(kaleidoscope_${ch} ${ch}.cpp)
    target_link_libraries(kaleidoscope_${ch} PRIVATE ${LLVM_WITH_OrcJIT})
endmacro ()

add_kaleidoscope_target(ch3)
add_kaleidoscope_target(ch4)
add_kaleidoscope_target(ch5)
add_kaleidoscope_target(ch6)
add_kaleidoscope_target(ch7)
add_kaleidoscope_target(ch8)
add_kaleidoscope_target(ch9)

if (ARCH_AMD64 OR ARCH_AARCH64)
    llvm_target_link_libraries(PRIVATE LLVMX86AsmParser LLVMAArch64AsmParser)
else()
    message(FATAL_ERROR "arch is not supported on llvm building")
endif()

