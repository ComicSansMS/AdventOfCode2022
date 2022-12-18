add_library(17_pyroclastic_flow STATIC pyroclastic_flow.hpp pyroclastic_flow.cpp)
target_include_directories(17_pyroclastic_flow PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(17_pyroclastic_flow PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent17 advent17.cpp)
target_link_libraries(advent17 PUBLIC 17_pyroclastic_flow)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(17_pyroclastic_flow_test pyroclastic_flow.t.cpp)
    target_link_libraries(17_pyroclastic_flow_test PUBLIC Catch 17_pyroclastic_flow)
    add_test(NAME TestAdvent17 COMMAND 17_pyroclastic_flow_test)
    enable_coverage(17_pyroclastic_flow)
endif()
