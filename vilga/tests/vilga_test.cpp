#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <cmath>
#include <vilga/vilga.h>

TEST_CASE("Basic portal construction test") {
  vilga::portal_settings settings;
  vilga::portal<int> portal(settings);
}

TEST_CASE("Basic tests for closed portal") {
  vilga::portal_settings settings;
  settings.mode = vilga::portal_settings::Mode::OFF;
  vilga::portal<int> portal(settings);
  portal.send(VILGA_VAL(42));
}

TEST_CASE("Basic tests for portal streaming") {
  vilga::portal_settings settings;
  settings.mode = vilga::portal_settings::Mode::STREAM;
  vilga::portal<int> portal(settings);
  for (int i = 0; i < 100; ++i) {
    portal.send(vilga::Val<float>("sin(i)", std::sin(static_cast<float>(i)/M_PI_2f32)));
    portal.send(VILGA_VAL(i));
  }
}