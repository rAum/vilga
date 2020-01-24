#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vilga/detail/data.hpp>
#include <vilga/detail/portal_spell.hpp>
#include <iostream>

static int global_i(0);

namespace vilga_detail {

void vilga_type_cast(const int& i, vilga_detail::data& data) {
  (void) data;
  global_i = i;
  std::cout << "convert " << i << std::endl;
}

}  // namespace vilga_detail

TEST_CASE("Basic tests for portal spell") {
  vilga_detail::data data;
  global_i = 0;

  SUBCASE("Simple test") {
    vilga_detail::portal_spell(13).cast_into(data);
    CHECK_EQ(global_i, 13);
  }

  SUBCASE("Temporal test") {
    // this does work:
    vilga_detail::portal_spell(3).cast_into(data);
    CHECK_EQ(global_i, 3);
    // it also works when there is print in constructor... and in debug.
    /// but this does not work (!):
    global_i = 0;
    __asm("# begin problem");
    vilga_detail::portal_spell tmp(5); // WHY 1582395837 ??
    tmp.cast_into(data);
    __asm("# end problem");
    CHECK_EQ(global_i, 5);

    global_i = 0;
    // BUT THIS WORKS TOO
    int v = 5;
    vilga_detail::portal_spell tmp2(v);
    tmp2.cast_into(data);
    CHECK_EQ(global_i, 5);
    // FIXME: investigate!
  }

  SUBCASE("As function(lambda) argument") {
    auto sink = [&data](vilga_detail::portal_spell spell) {
      spell.cast_into(data);
    };
    sink(9);
      CHECK_EQ(global_i, 9);

    sink(53);
      CHECK_EQ(global_i, 53);
  }
}