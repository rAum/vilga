#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <cmath>
#include <vilga/vilga.h>
#include <zmq.hpp>

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
  zmq::context_t c(1);
  zmq::socket_t s(c, ZMQ_SUB);
  s.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  s.connect("tcp://127.0.0.1:3349");

  vilga::portal_settings settings;
  settings.mode = vilga::portal_settings::Mode::STREAM;
  vilga::portal<int> portal(settings);

  const int send_messages = 100;
  for (int i = 0; i < send_messages / 2; ++i) {
    portal.send(vilga::Val<float>("sin(i)", std::sin(static_cast<float>(i)/M_PI_2f32)));
    portal.send(VILGA_VAL(i));
  }

  auto begin = std::chrono::steady_clock::now();
  auto timeout = std::chrono::seconds(1);
  int recieived_messages = 0;
  while ((std::chrono::steady_clock::now() - begin) < timeout) {
    zmq::message_t m;
    if (s.recv(&m, ZMQ_NOBLOCK)) {
      ++recieived_messages;
      if (recieived_messages == 200) break;
    }
  }
  CHECK_EQ(recieived_messages, send_messages);
}