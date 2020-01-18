#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <cmath>
#include <vilga/vilga.h>
#include <zmq.hpp>
#include <thread>

zmq::context_t test_context(1);

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
  zmq::socket_t s(test_context, ZMQ_SUB);
  s.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  s.connect("tcp://127.0.0.1:3349");

  vilga::portal_settings settings;
  settings.mode = vilga::portal_settings::Mode::STREAM;
  vilga::portal<int> portal(settings);

  // wait for subscriber connection to happen before publishing messages.
  // otherwise the messages are dropped :(
  std::this_thread::sleep_for(std::chrono::seconds(1));

  const int send_messages = 100;
  for (int i = 0; i < send_messages; ++i) {
    portal.send(vilga::Val<float>("sin(i)", std::sin(static_cast<float>(i)/M_PI_2f32)));
  }

  auto begin = std::chrono::steady_clock::now();
  auto timeout = std::chrono::seconds(1);
  int received_messages = 0;
  while ((std::chrono::steady_clock::now() - begin) < timeout) {
    zmq::message_t m;
    if (s.recv(&m, ZMQ_NOBLOCK)) {
      ++received_messages;
    }
  }
  CHECK_EQ(received_messages, send_messages);
}

TEST_CASE("Multithreaded test for portal streaming") {
  zmq::socket_t s(test_context, ZMQ_SUB);
  s.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  s.connect("tcp://127.0.0.1:3349");

  // wait for subscriber connection to happen before publishing messages.
  // otherwise the messages are dropped :(
  std::this_thread::sleep_for(std::chrono::seconds(1));

  constexpr int send_messages = 100;
  constexpr int n_threads = 10;
  std::vector<std::thread> threads;
  threads.reserve(n_threads);
  for (int i = 0; i < n_threads; ++i) {
    threads.emplace_back([send_messages]() {
      vilga::portal_settings settings;
      settings.mode = vilga::portal_settings::Mode::STREAM;
      vilga::portal<int> portal(settings);

      for (int i = 0; i < send_messages; ++i) {
        portal.send(vilga::Val<int>("i", i));
      }
    });
  }

  auto begin = std::chrono::steady_clock::now();
  auto timeout = std::chrono::seconds(1);
  int received_messages = 0;
  while ((std::chrono::steady_clock::now() - begin) < timeout) {
    zmq::message_t m;
    if (s.recv(&m, ZMQ_NOBLOCK)) {
      ++received_messages;
    }
  }

  for (auto& thread : threads) {
    thread.join();
  }

  CHECK_EQ(received_messages, send_messages * n_threads);
}