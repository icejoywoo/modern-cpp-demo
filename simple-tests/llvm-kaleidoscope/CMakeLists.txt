if (TARGET ch_contrib::llvm)
    set(USE_EMBEDDED_COMPILER 1)
endif()

if (USE_EMBEDDED_COMPILER)
    #add_executable(toy toy.cpp)
    #target_link_libraries(toy PRIVATE ch_contrib::llvm)
endif()