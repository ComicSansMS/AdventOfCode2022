add_library(25_full_oft_hot_air STATIC full_oft_hot_air.hpp full_oft_hot_air.cpp)
target_include_directories(25_full_oft_hot_air PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(25_full_oft_hot_air PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent25 advent25.cpp)
target_link_libraries(advent25 PUBLIC 25_full_oft_hot_air)

if (DEFINED ENV{CI})
    target_compile_definitions(advent25 PUBLIC RUNS_IN_CI)
endif()

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(25_full_oft_hot_air_test full_oft_hot_air.t.cpp)
    target_link_libraries(25_full_oft_hot_air_test PUBLIC Catch 25_full_oft_hot_air)
    add_test(NAME TestAdvent25 COMMAND 25_full_oft_hot_air_test)
    enable_coverage(25_full_oft_hot_air)
endif()
