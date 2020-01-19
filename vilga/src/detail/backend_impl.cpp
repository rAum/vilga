#include <iostream>
#include <vilga_impl/backend.hpp>

namespace vilga_detail {

//  If a publisher has no connected subscribers, then it will simply drop all messages
//   so maybe it should be switched to more reliable req/rep .. but this will hurt performance..
//  Assuming TCP: when subscriber is slow, messages will queue up on the publisher
//    this is not great either, because it can put down publisher..
//  Topic filtering happens at the subscriber side, not the publisher side AFAIK
//    so no reason to use it I think..
backend::impl::impl()
  : context_(1)
  , sink_socket_(context_, ZMQ_PULL)
  , cmd_socket_(context_, ZMQ_REP)
  , publish_socket_(context_, ZMQ_PUB)
  , killer_socket_(context_, ZMQ_REQ) {
  sink_socket_.bind(kGathererTopic);
  cmd_socket_.bind("tcp://127.0.0.1:3348");
  killer_socket_.connect("tcp://127.0.0.1:3348");
  publish_socket_.bind("tcp://127.0.0.1:3349");

  // late subscriber joiner sleep, this is unfortunate but otherwise it's tricky to not drop messages.
  // probably should start with PAUSE and wait for command to start and maybe even
  // setup publisher... need to think more about it.
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  /// FIXME: this is a very dirty & quick draft
  backend_operation_ = std::thread([this]() {
    std::array<zmq::pollitem_t, 2> items; // NOLINT
    items[0].socket = (void*)cmd_socket_;
    items[0].events = 0;
    items[0].fd = 0;
    items[0].events = ZMQ_POLLIN;

    items[1].socket = (void*)sink_socket_;
    items[1].events = 0;
    items[1].fd = 0;
    items[1].events = ZMQ_POLLIN;

    do {
      int status = zmq::poll(items.data(), items.size(), std::chrono::milliseconds(2));
      if (status < 0) break;

      if (items[0].revents & ZMQ_POLLIN) {
        bool die = false;
        zmq::message_t cmd_message;
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
        sink_socket_.recv(&sink_message);
        publish_socket_.send(sink_message);
        // we might have more messages... try to fetch at most 31 messages before polling
        for (int i = 0; i < 31; ++i) {
          zmq::message_t tmp;
          if (sink_socket_.recv(&tmp, ZMQ_NOBLOCK)) {
            publish_socket_.send(sink_message);
          } else {
            break; // okay! no more for now, time to poll
          }
        }
      }

    } while (true);

  });
}

backend::impl::~impl() {
  zmq_send_const((void*)killer_socket_, "DIE", 4, 0);
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