add_library(14_regolith_reservoir STATIC regolith_reservoir.hpp regolith_reservoir.cpp)
target_include_directories(14_regolith_reservoir PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(14_regolith_reservoir PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent14 advent14.cpp)
target_link_libraries(advent14 PUBLIC 14_regolith_reservoir)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(14_regolith_reservoir_test regolith_reservoir.t.cpp)
    target_link_libraries(14_regolith_reservoir_test PUBLIC Catch 14_regolith_reservoir)
    add_test(NAME TestAdvent14 COMMAND 14_regolith_reservoir_test)
    enable_coverage(14_regolith_reservoir)
endif()
