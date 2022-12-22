add_library(22_monkey_map STATIC monkey_map.hpp monkey_map.cpp)
target_include_directories(22_monkey_map PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(22_monkey_map PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent22 advent22.cpp)
target_link_libraries(advent22 PUBLIC 22_monkey_map)

if (DEFINED ENV{CI})
    target_compile_definitions(advent22 PUBLIC RUNS_IN_CI)
endif()

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(22_monkey_map_test monkey_map.t.cpp)
    target_link_libraries(22_monkey_map_test PUBLIC Catch 22_monkey_map)
    add_test(NAME TestAdvent22 COMMAND 22_monkey_map_test)
    enable_coverage(22_monkey_map)
endif()
