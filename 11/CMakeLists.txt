add_library(11_monkey_in_the_middle STATIC monkey_in_the_middle.hpp monkey_in_the_middle.cpp)
target_include_directories(11_monkey_in_the_middle PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(11_monkey_in_the_middle PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent11 advent11.cpp)
target_link_libraries(advent11 PUBLIC 11_monkey_in_the_middle)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(11_monkey_in_the_middle_test monkey_in_the_middle.t.cpp)
    target_link_libraries(11_monkey_in_the_middle_test PUBLIC Catch 11_monkey_in_the_middle)
    add_test(NAME TestAdvent11 COMMAND 11_monkey_in_the_middle_test)
    enable_coverage(11_monkey_in_the_middle)
endif()
