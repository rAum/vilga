#pragma once
#ifndef VILGA_DETAIL_BACKEND_HPP
#define VILGA_DETAIL_BACKEND_HPP

#include <memory>
#include <vilga/detail/macros.hpp>

namespace vilga_detail VILGA_EXPORT {

class data;
struct vilga_backend_initializer;

/**
 * Single entry point which routes data into destinations.
 */
class backend {
private:
  backend();
  ~backend();
public:
  friend struct vilga_backend_initializer;

  void consume(data&& data);

private:
  struct impl;
  std::unique_ptr<backend::impl> impl_;
};

/// global reference to backend
extern backend& backend_instance;

/// static backend initializer for every translation unit
static struct vilga_backend_initializer {
  vilga_backend_initializer() noexcept;
  ~vilga_backend_initializer() noexcept;
} vilga_backend_init;

}  // namespace vilga_detail

#endif  // VILGA_DETAIL_BACKEND_HPP
