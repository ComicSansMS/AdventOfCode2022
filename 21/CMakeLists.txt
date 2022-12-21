add_library(21_monkey_math STATIC monkey_math.hpp monkey_math.cpp)
target_include_directories(21_monkey_math PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(21_monkey_math PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent21 advent21.cpp)
target_link_libraries(advent21 PUBLIC 21_monkey_math)

if (DEFINED ENV{CI})
    target_compile_definitions(advent21 PUBLIC RUNS_IN_CI)
endif()

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(21_monkey_math_test monkey_math.t.cpp)
    target_link_libraries(21_monkey_math_test PUBLIC Catch 21_monkey_math)
    add_test(NAME TestAdvent21 COMMAND 21_monkey_math_test)
    enable_coverage(21_monkey_math)
endif()
