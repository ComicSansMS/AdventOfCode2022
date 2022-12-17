add_library(16_proboscidea_volcanium STATIC proboscidea_volcanium.hpp proboscidea_volcanium.cpp)
target_include_directories(16_proboscidea_volcanium PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(16_proboscidea_volcanium PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent16 advent16.cpp)
target_link_libraries(advent16 PUBLIC 16_proboscidea_volcanium)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(16_proboscidea_volcanium_test proboscidea_volcanium.t.cpp)
    target_link_libraries(16_proboscidea_volcanium_test PUBLIC Catch 16_proboscidea_volcanium)
    add_test(NAME TestAdvent16 COMMAND 16_proboscidea_volcanium_test)
    enable_coverage(16_proboscidea_volcanium)
endif()
