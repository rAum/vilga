#pragma once
#ifndef VILGA_DETAIL_BACKEND_HPP
#define VILGA_DETAIL_BACKEND_HPP

#include <memory>
#include <vilga/detail/macros.hpp>

namespace vilga_detail VILGA_EXPORT {

class data;

/**
 * Single entry point which routes data into destinations.
 */
class backend {
public:
  backend();
  ~backend();

  backend(backend&& other) noexcept;
  void operator=(backend&& other) noexcept;

  void consume(data&& data);

private:
  struct impl;
  std::unique_ptr<backend::impl> impl_;
};

}  // namespace vilga_detail

#endif  // VILGA_DETAIL_BACKEND_HPP
