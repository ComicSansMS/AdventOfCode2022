add_library(06_tuning_trouble STATIC tuning_trouble.hpp tuning_trouble.cpp)
target_include_directories(06_tuning_trouble PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(06_tuning_trouble PUBLIC fmt::fmt range-v3)
add_executable(advent06 advent06.cpp)
target_link_libraries(advent06 PUBLIC 06_tuning_trouble)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(06_tuning_trouble_test tuning_trouble.t.cpp)
    target_link_libraries(06_tuning_trouble_test PUBLIC Catch 06_tuning_trouble)
    add_test(NAME TestAdvent06 COMMAND 06_tuning_trouble_test)
    enable_coverage(06_tuning_trouble)
endif()
