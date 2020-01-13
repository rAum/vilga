#include <iostream>
#include <vilga_impl/backend.hpp>

namespace vilga_detail {

backend::impl::impl()
  : context_(1)
  , sink_socket_(context_, ZMQ_PULL)
  , cmd_socket_(context_, ZMQ_REP)
  , publish_socket_(context_, ZMQ_PUB)
  , killer_socket_(context_, ZMQ_REQ) {
  sink_socket_.bind(kGathererTopic);
  cmd_socket_.bind("tcp://127.0.0.1:3348");
  publish_socket_.bind("tcp://*:3349");
  killer_socket_.connect("tcp://127.0.0.1:3348");

  /// FIXME: this is a very dirty & quick draft
  backend_operation_ = std::thread([this]() {
    std::array<zmq::pollitem_t, 2> items; // NOLINT
    items[0].socket = cmd_socket_;
    items[0].events = 0;
    items[0].fd = 0;
    items[0].events = ZMQ_POLLIN;

    items[1].socket = sink_socket_;
    items[1].events = 0;
    items[1].fd = 0;
    items[1].events = ZMQ_POLLIN;

    zmq::message_t cmd_message;
    do {
      int status = zmq::poll(items.data(), items.size(), std::chrono::milliseconds(2));
      if (status < 0) break;
      cmd_message.rebuild(16);

      if (items[0].revents & ZMQ_POLLIN) {
        bool die = false;
        while (cmd_socket_.recv(&cmd_message, ZMQ_NOBLOCK)) {
          std::cout << "Got command of size " << cmd_message.size() << std::endl;
          if (cmd_message.size() == 4) {
            die = true;
            break;
          } else {
            cmd_socket_.send("", 0);
          }
        }
        if (die) {
          std::cout << "Die." << std::endl;
          cmd_socket_.send("", 0);
          // TODO: should try to pump out all messages
          return;
        }
      }

      if (items[1].revents & ZMQ_POLLIN) {
        zmq::message_t sink_message;
        while (sink_socket_.recv(&sink_message, ZMQ_NOBLOCK)) {
          std::cout << "pushing message from sink to publisher [" << sink_message.size() << "] " << std::endl;
          if (!publish_socket_.send(sink_message, ZMQ_NOBLOCK)) {
            // failed to send. let's retry..
            std::this_thread::sleep_for(std::chrono::nanoseconds(30));
            if (!publish_socket_.send(sink_message, ZMQ_NOBLOCK)) {
              std::cout << "Message dropped. Sorry." << std::endl;
            }
          }
        }
      }

    } while (true);

  });
}

backend::impl::~impl() {
  consume_sockets_.die();
  std::this_thread::sleep_for(std::chrono::milliseconds(4));
  zmq_send_const(killer_socket_, "DIE", 4, 0);
  zmq::message_t ok(4);
  killer_socket_.recv(&ok);
  killer_socket_.close();
  sink_socket_.close();
  if (backend_operation_.joinable()) {
    backend_operation_.join();
  }
  cmd_socket_.close();
  publish_socket_.close();
  context_.close();
}

}  // namespace vilga_detail