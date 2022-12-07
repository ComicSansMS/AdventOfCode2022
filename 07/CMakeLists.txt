add_library(07_no_space_left_on_device STATIC no_space_left_on_device.hpp no_space_left_on_device.cpp)
target_include_directories(07_no_space_left_on_device PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(07_no_space_left_on_device PUBLIC fmt::fmt range-v3)
add_executable(advent07 advent07.cpp)
target_link_libraries(advent07 PUBLIC 07_no_space_left_on_device)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(07_no_space_left_on_device_test no_space_left_on_device.t.cpp)
    target_link_libraries(07_no_space_left_on_device_test PUBLIC Catch 07_no_space_left_on_device)
    add_test(NAME TestAdvent07 COMMAND 07_no_space_left_on_device_test)
    enable_coverage(07_no_space_left_on_device)
endif()
