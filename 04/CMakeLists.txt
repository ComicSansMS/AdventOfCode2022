add_library(04_camp_cleanup STATIC camp_cleanup.hpp camp_cleanup.cpp)
target_include_directories(04_camp_cleanup PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(04_camp_cleanup PUBLIC fmt::fmt range-v3)
add_executable(advent04 advent04.cpp)
target_link_libraries(advent04 PUBLIC 04_camp_cleanup)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(04_camp_cleanup_test camp_cleanup.t.cpp)
    target_link_libraries(04_camp_cleanup_test PUBLIC Catch 04_camp_cleanup)
    add_test(NAME TestAdvent04 COMMAND 04_camp_cleanup_test)
    enable_coverage(04_camp_cleanup)
endif()
