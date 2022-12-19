add_library(19_not_enough_minerals STATIC not_enough_minerals.hpp not_enough_minerals.cpp)
target_include_directories(19_not_enough_minerals PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(19_not_enough_minerals PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent19 advent19.cpp)
target_link_libraries(advent19 PUBLIC 19_not_enough_minerals)

if (DEFINED ENV{CI})
    target_compile_definitions(advent19 PUBLIC RUNS_IN_CI)
endif()

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(19_not_enough_minerals_test not_enough_minerals.t.cpp)
    target_link_libraries(19_not_enough_minerals_test PUBLIC Catch 19_not_enough_minerals)
    add_test(NAME TestAdvent19 COMMAND 19_not_enough_minerals_test)
    enable_coverage(19_not_enough_minerals)
endif()
