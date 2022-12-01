add_library(01_calorie_counting STATIC calorie_counting.hpp calorie_counting.cpp)
target_include_directories(01_calorie_counting PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(01_calorie_counting PUBLIC fmt::fmt range-v3)
add_executable(advent01 advent01.cpp)
target_link_libraries(advent01 PUBLIC 01_calorie_counting)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(01_calorie_counting_test calorie_counting.t.cpp)
    target_link_libraries(01_calorie_counting_test PUBLIC Catch 01_calorie_counting)
    add_test(NAME TestAdvent01 COMMAND 01_calorie_counting_test)
    enable_coverage(01_calorie_counting)
endif()
