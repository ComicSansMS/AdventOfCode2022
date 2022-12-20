add_library(20_grove_positioning_system STATIC grove_positioning_system.hpp grove_positioning_system.cpp)
target_include_directories(20_grove_positioning_system PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(20_grove_positioning_system PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent20 advent20.cpp)
target_link_libraries(advent20 PUBLIC 20_grove_positioning_system)

if (DEFINED ENV{CI})
    target_compile_definitions(advent20 PUBLIC RUNS_IN_CI)
endif()

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(20_grove_positioning_system_test grove_positioning_system.t.cpp)
    target_link_libraries(20_grove_positioning_system_test PUBLIC Catch 20_grove_positioning_system)
    add_test(NAME TestAdvent20 COMMAND 20_grove_positioning_system_test)
    enable_coverage(20_grove_positioning_system)
endif()
