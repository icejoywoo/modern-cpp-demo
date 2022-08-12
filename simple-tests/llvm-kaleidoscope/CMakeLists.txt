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

# refer ClickHouse src/CMakeLists.txt dbms_target_link_libraries
list (APPEND all_modules toy)
macro (llvm_target_link_libraries)
    foreach (module ${all_modules})
        target_link_libraries (${module} ${ARGN})
    endforeach ()
endmacro ()

add_executable(kaleidoscope_ch2 ch2.cpp)

add_executable(kaleidoscope_ch4 ch4.cpp)
list (APPEND all_modules kaleidoscope_ch4)
target_link_libraries(kaleidoscope_ch4 PRIVATE ch_contrib::llvm
    LLVMOrcJIT
    ${CMAKE_DL_LIBS})

add_executable(kaleidoscope_ch5 ch5.cpp)
list (APPEND all_modules kaleidoscope_ch5)
target_link_libraries(kaleidoscope_ch5 PRIVATE ch_contrib::llvm
    LLVMOrcJIT
    ${CMAKE_DL_LIBS})

if (ARCH_AMD64)
    llvm_target_link_libraries(PRIVATE LLVMX86AsmParser)
elseif(ARCH_AARCH64)
    llvm_target_link_libraries(PRIVATE LLVMAArch64AsmParser)
else()
    message(FATAL_ERROR "arch is not supported on llvm building")
endif()

