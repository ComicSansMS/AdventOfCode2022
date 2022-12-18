add_library(18_boiling_boulders STATIC boiling_boulders.hpp boiling_boulders.cpp)
target_include_directories(18_boiling_boulders PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(18_boiling_boulders PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent18 advent18.cpp)
target_link_libraries(advent18 PUBLIC 18_boiling_boulders)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(18_boiling_boulders_test boiling_boulders.t.cpp)
    target_link_libraries(18_boiling_boulders_test PUBLIC Catch 18_boiling_boulders)
    add_test(NAME TestAdvent18 COMMAND 18_boiling_boulders_test)
    enable_coverage(18_boiling_boulders)
endif()
