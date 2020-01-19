#pragma once
#ifndef VILGA_IMPL_BACKEND_HPP
#define VILGA_IMPL_BACKEND_HPP

#include <thread>
#include <zmq.hpp>
#include <vilga/detail/backend.hpp>
#include <vilga/detail/data.hpp>
#include <vilga_impl/autoconnect_socket.hpp>

namespace vilga_detail {

class backend::impl {
public:
  impl();
  ~impl();

  void operator=(const impl&) = delete;

  /**
   * Passes data into other thread to reduce blocking in current thread and allow async io.
   * @param data
   */
  void consume(data&& data) {
    // TODO: investigate later lock-free MPSC, it probably could be faster than zmq
    thread_local autoconnect_socket this_thread_socket(context_);
    // It still is going to block if the destination socket is full/not listening...
    // for now it seems as a good prototype strategy.
    // Data is going to be copied here as we cross threads boundary
    this_thread_socket->send(data.data_.bytes_.get(), data.data_.size_, ZMQ_NOBLOCK);
  }

private:
  zmq::context_t context_;
  zmq::socket_t sink_socket_;
  zmq::socket_t cmd_socket_;
  zmq::socket_t publish_socket_;
  zmq::socket_t killer_socket_;
  std::thread backend_operation_;
};

}  // namespace vilga_detail

#endif //  VILGA_IMPL_BACKEND_HPP
