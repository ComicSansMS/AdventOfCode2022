add_library(23_unstable_diffusion STATIC unstable_diffusion.hpp unstable_diffusion.cpp)
target_include_directories(23_unstable_diffusion PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(23_unstable_diffusion PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent23 advent23.cpp)
target_link_libraries(advent23 PUBLIC 23_unstable_diffusion)

if (DEFINED ENV{CI})
    target_compile_definitions(advent23 PUBLIC RUNS_IN_CI)
endif()

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(23_unstable_diffusion_test unstable_diffusion.t.cpp)
    target_link_libraries(23_unstable_diffusion_test PUBLIC Catch 23_unstable_diffusion)
    add_test(NAME TestAdvent23 COMMAND 23_unstable_diffusion_test)
    enable_coverage(23_unstable_diffusion)
endif()
