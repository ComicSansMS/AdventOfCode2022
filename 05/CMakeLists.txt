add_library(05_supply_stacks STATIC supply_stacks.hpp supply_stacks.cpp)
target_include_directories(05_supply_stacks PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(05_supply_stacks PUBLIC fmt::fmt range-v3)
add_executable(advent05 advent05.cpp)
target_link_libraries(advent05 PUBLIC 05_supply_stacks)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(05_supply_stacks_test supply_stacks.t.cpp)
    target_link_libraries(05_supply_stacks_test PUBLIC Catch 05_supply_stacks)
    add_test(NAME TestAdvent05 COMMAND 05_supply_stacks_test)
    enable_coverage(05_supply_stacks)
endif()
