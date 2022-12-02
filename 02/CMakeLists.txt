add_library(02_rock_paper_scissors STATIC rock_paper_scissors.hpp rock_paper_scissors.cpp)
target_include_directories(02_rock_paper_scissors PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(02_rock_paper_scissors PUBLIC fmt::fmt range-v3)
add_executable(advent02 advent02.cpp)
target_link_libraries(advent02 PUBLIC 02_rock_paper_scissors)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(02_rock_paper_scissors_test rock_paper_scissors.t.cpp)
    target_link_libraries(02_rock_paper_scissors_test PUBLIC Catch 02_rock_paper_scissors)
    add_test(NAME TestAdvent02 COMMAND 02_rock_paper_scissors_test)
    enable_coverage(02_rock_paper_scissors)
endif()
