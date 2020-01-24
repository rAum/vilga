// Simple app for testing during development.

#include <vilga/vilga.h>
#include <cmath>

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  vilga::portal_settings settings{};
  settings.mode = vilga::portal_settings::Mode::STREAM;
  vilga::portal<int> portal(settings);

  for (int i = 0; i < 100; ++i) {
    portal.send(std::sin(i/M_PI_2f32));//vilga::Val<float>("sin(i)", std::sin(i/M_PI_2f32)));
    portal.send(i);//VILGA_VAL(i));
  }

  return 0;
}
