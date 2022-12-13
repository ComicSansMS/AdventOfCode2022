add_library(12_hill_climbing_algorithm STATIC hill_climbing_algorithm.hpp hill_climbing_algorithm.cpp)
target_include_directories(12_hill_climbing_algorithm PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(12_hill_climbing_algorithm PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent12 advent12.cpp)
target_link_libraries(advent12 PUBLIC 12_hill_climbing_algorithm)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(12_hill_climbing_algorithm_test hill_climbing_algorithm.t.cpp)
    target_link_libraries(12_hill_climbing_algorithm_test PUBLIC Catch 12_hill_climbing_algorithm)
    add_test(NAME TestAdvent12 COMMAND 12_hill_climbing_algorithm_test)
    enable_coverage(12_hill_climbing_algorithm)
endif()
