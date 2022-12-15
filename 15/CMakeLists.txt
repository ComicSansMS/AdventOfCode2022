add_library(15_beacon_exclusion_zone STATIC beacon_exclusion_zone.hpp beacon_exclusion_zone.cpp)
target_include_directories(15_beacon_exclusion_zone PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(15_beacon_exclusion_zone PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent15 advent15.cpp)
target_link_libraries(advent15 PUBLIC 15_beacon_exclusion_zone)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(15_beacon_exclusion_zone_test beacon_exclusion_zone.t.cpp)
    target_link_libraries(15_beacon_exclusion_zone_test PUBLIC Catch 15_beacon_exclusion_zone)
    add_test(NAME TestAdvent15 COMMAND 15_beacon_exclusion_zone_test)
    enable_coverage(15_beacon_exclusion_zone)
endif()
