#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vilga/detail/data.hpp>
#include <vilga/detail/portal_spell.hpp>

static int global_i;

namespace vilga_detail {

void vilga_type_cast(const int& i, vilga_detail::data& data) {
  (void) data;
  global_i = i;
}

}  // namespace vilga_detail

TEST_CASE("Basic tests for portal spell") {
  vilga_detail::portal_spell test(4);
  vilga_detail::data data;
  test.cast_into(data);
  CHECK_EQ(global_i, 4);

  auto sink = [&data](vilga_detail::portal_spell spell) {
    spell.cast_into(data);
  };
  sink(5);
  CHECK_EQ(global_i, 5);
  sink(53);
  CHECK_EQ(global_i, 53);
}