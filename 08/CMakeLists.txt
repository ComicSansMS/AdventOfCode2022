add_library(08_treetop_tree_house STATIC treetop_tree_house.hpp treetop_tree_house.cpp)
target_include_directories(08_treetop_tree_house PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(08_treetop_tree_house PUBLIC fmt::fmt range-v3 std::mdspan)
add_executable(advent08 advent08.cpp)
target_link_libraries(advent08 PUBLIC 08_treetop_tree_house)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(08_treetop_tree_house_test treetop_tree_house.t.cpp)
    target_link_libraries(08_treetop_tree_house_test PUBLIC Catch 08_treetop_tree_house)
    add_test(NAME TestAdvent08 COMMAND 08_treetop_tree_house_test)
    enable_coverage(08_treetop_tree_house)
endif()
