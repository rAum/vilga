#pragma once
#ifndef VILGA_IMPL_THREAD_LOCAL_SOCKETS_HPP
#define VILGA_IMPL_THREAD_LOCAL_SOCKETS_HPP

#include <zmq.hpp>
#include <vilga_impl/backend_config.hpp>

namespace vilga_detail {

/**
 * This is a wrapper around zmq::socket_t which ensures autoconnecton/disconnection
 * Assuming that zmq::context outlives those sockets (otherwise it would be bad, very bad)
 */
class autoconnect_socket final {
public:
  explicit autoconnect_socket(zmq::context_t& context, int zmq_type = ZMQ_PUSH)
  : socket_(context, zmq_type) {
    // Pending messages shall not be discarded after a call to close()
    socket_.setsockopt(ZMQ_LINGER, -1);
    socket_.connect(kGathererTopic);
  }

  const zmq::socket_t& operator*() const noexcept {
    return socket_;
  }

  zmq::socket_t& operator*() noexcept {
    return socket_;
  }

  const zmq::socket_t* operator->() const noexcept {
    return &socket_;
  }

  zmq::socket_t* operator->() noexcept {
    return &socket_;
  }

  ~autoconnect_socket() {
    socket_.disconnect(kGathererTopic);
    socket_.close();
  }

private:
  zmq::socket_t socket_;
};

}  // namespace vilga_detail

#endif  // VILGA_IMPL_THREAD_LOCAL_SOCKETS_HPP
