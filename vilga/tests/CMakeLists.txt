add_executable(vilga_test vilga_test.cpp)
target_link_libraries(vilga_test PRIVATE vilga::vilga doctest::doctest)
add_test(test_vilga_test vilga_test)

add_executable(spell_test spell_test.cpp)
target_link_libraries(spell_test PRIVATE vilga::vilga doctest::doctest)
add_test(test_spell spell_test)