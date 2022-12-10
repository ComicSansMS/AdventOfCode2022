add_library(10_cathode_ray_tube STATIC cathode_ray_tube.hpp cathode_ray_tube.cpp)
target_include_directories(10_cathode_ray_tube PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(10_cathode_ray_tube PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent10 advent10.cpp)
target_link_libraries(advent10 PUBLIC 10_cathode_ray_tube)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(10_cathode_ray_tube_test cathode_ray_tube.t.cpp)
    target_link_libraries(10_cathode_ray_tube_test PUBLIC Catch 10_cathode_ray_tube)
    add_test(NAME TestAdvent10 COMMAND 10_cathode_ray_tube_test)
    enable_coverage(10_cathode_ray_tube)
endif()
