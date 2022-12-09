add_library(09_rope_bridge STATIC rope_bridge.hpp rope_bridge.cpp)
target_include_directories(09_rope_bridge PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(09_rope_bridge PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent09 advent09.cpp)
target_link_libraries(advent09 PUBLIC 09_rope_bridge)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(09_rope_bridge_test rope_bridge.t.cpp)
    target_link_libraries(09_rope_bridge_test PUBLIC Catch 09_rope_bridge)
    add_test(NAME TestAdvent09 COMMAND 09_rope_bridge_test)
    enable_coverage(09_rope_bridge)
endif()
