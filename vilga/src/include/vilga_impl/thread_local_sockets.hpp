#pragma once
#ifndef VILGA_IMPL_THREAD_LOCAL_SOCKETS_HPP
#define VILGA_IMPL_THREAD_LOCAL_SOCKETS_HPP

#include <mutex>
#include <atomic>
#include <deque>
#include <zmq.hpp>
#include <vilga_impl/backend_config.hpp>

namespace vilga_detail {

/**
 * Wonder if this is correct.. basically it should be fast lookup for small number of threads.
 * Should be concurrent hash map... but it's more complicated in implementation.
 * The problem is that ideally socket should be thread_local but it depends on context...
 * so if context is to die I see no other way to kill socket...
 * Of course if context would be global.. then there is no problem, but! it would hang on tests.
 * At least for now backend is coupled with portal lifetime so it requires a solution.
 * Especially if I would like to get rid of zeromq and provide alternative backend implementation.
 * Anyway I am doing it for tests and prototyping so let it be, only I will get hurt.
 */
class thread_local_sockets {
public:

  void die() noexcept {
    is_dead_ = true;
    count_ = 0;
    lock_for_write_.lock();
    sockets_.clear();
    lock_for_write_.unlock();
  }

  // if exception would happen... please die.
  zmq::socket_t* get_socket_for_this_thread(zmq::context_t& context) noexcept {
    if (is_dead_)
      return nullptr;

    thread_local auto id = std::this_thread::get_id();
    const size_t max = count_;
    for (size_t i = 0; i < max; ++i) {
      if (sockets_[i].first == id) {
        return &sockets_[i].second;
      }
    }

    zmq::socket_t* ref = nullptr;
    lock_for_write_.lock();
    if (is_dead_) return nullptr;
    sockets_.emplace_back(id, zmq::socket_t(context, ZMQ_PUSH));
    ref = &sockets_.back().second;
    ++count_;
    lock_for_write_.unlock();
    ref->connect(kGathererTopic);
    return ref;
  }

private:
  std::deque<std::pair<std::thread::id, zmq::socket_t>> sockets_;
  std::atomic<bool> is_dead_ = ATOMIC_FLAG_INIT;
  std::atomic<size_t> count_ = ATOMIC_VAR_INIT(0);
  std::mutex lock_for_write_;
};

}  // namespace vilga_detail

#endif  // VILGA_IMPL_THREAD_LOCAL_SOCKETS_HPP
