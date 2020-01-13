
add_executable(vilga_test vilga_test.cpp)
target_link_libraries(vilga_test PRIVATE vilga::vilga doctest::doctest)