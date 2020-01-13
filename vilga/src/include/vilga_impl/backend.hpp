#pragma once
#ifndef VILGA_IMPL_BACKEND_HPP
#define VILGA_IMPL_BACKEND_HPP

#include <thread>
#include <zmq.hpp>
#include <vilga/detail/backend.hpp>
#include <vilga/detail/data.hpp>
#include <vilga_impl/thread_local_sockets.hpp>

namespace vilga_detail {

class backend::impl {
public:
  impl();
  ~impl();

  void operator=(const impl&) = delete;

  // TODO: think about multiple data.. would be handy to get this_thread_socket exactly once
  // for multiple parts
  void consume(data&& data) {
    auto this_thread_socket = consume_sockets_.get_socket_for_this_thread(context_);
    if (!this_thread_socket)
      return; // probably warning..
    // It still is going to block if the destination socket is full/not listening...
    // for now it seems as a good prototype strategy.
    // Data is going to be copied here as we cross threads boundary
    this_thread_socket->send(data.data_.bytes_.get(), data.data_.size_, ZMQ_NOBLOCK);
  }

private:
  zmq::context_t context_;
  thread_local_sockets consume_sockets_;
  zmq::socket_t sink_socket_;
  zmq::socket_t cmd_socket_;
  zmq::socket_t publish_socket_;
  zmq::socket_t killer_socket_;
  std::thread backend_operation_;
};

}  // namespace vilga_detail

#endif //  VILGA_IMPL_BACKEND_HPP
