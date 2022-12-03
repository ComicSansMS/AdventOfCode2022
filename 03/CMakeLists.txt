add_library(03_rucksack_reorganization STATIC rucksack_reorganization.hpp rucksack_reorganization.cpp)
target_include_directories(03_rucksack_reorganization PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(03_rucksack_reorganization PUBLIC fmt::fmt range-v3)
add_executable(advent03 advent03.cpp)
target_link_libraries(advent03 PUBLIC 03_rucksack_reorganization)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(03_rucksack_reorganization_test rucksack_reorganization.t.cpp)
    target_link_libraries(03_rucksack_reorganization_test PUBLIC Catch 03_rucksack_reorganization)
    add_test(NAME TestAdvent03 COMMAND 03_rucksack_reorganization_test)
    enable_coverage(03_rucksack_reorganization)
endif()
