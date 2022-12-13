add_library(13_distress_signal STATIC distress_signal.hpp distress_signal.cpp)
target_include_directories(13_distress_signal PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(13_distress_signal PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent13 advent13.cpp)
target_link_libraries(advent13 PUBLIC 13_distress_signal)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(13_distress_signal_test distress_signal.t.cpp)
    target_link_libraries(13_distress_signal_test PUBLIC Catch 13_distress_signal)
    add_test(NAME TestAdvent13 COMMAND 13_distress_signal_test)
    enable_coverage(13_distress_signal)
endif()
