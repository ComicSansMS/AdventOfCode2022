add_library(24_blizzard_basin STATIC blizzard_basin.hpp blizzard_basin.cpp)
target_include_directories(24_blizzard_basin PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(24_blizzard_basin PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent24 advent24.cpp)
target_link_libraries(advent24 PUBLIC 24_blizzard_basin)

if (DEFINED ENV{CI})
    target_compile_definitions(advent24 PUBLIC RUNS_IN_CI)
endif()

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(24_blizzard_basin_test blizzard_basin.t.cpp)
    target_link_libraries(24_blizzard_basin_test PUBLIC Catch 24_blizzard_basin)
    add_test(NAME TestAdvent24 COMMAND 24_blizzard_basin_test)
    enable_coverage(24_blizzard_basin)
endif()
