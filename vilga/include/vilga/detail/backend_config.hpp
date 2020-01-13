#pragma once
#ifndef VILGA_DETAIL_BACKEND_CONFIG_HPP
#define VILGA_DETAIL_BACKEND_CONFIG_HPP

#include <string>
#include <vilga/detail/macros.hpp>

namespace vilga_detail VILGA_EXPORT {

struct backend_config {
  std::string zmq_protocol; // FIXME: leaking implementation detail.
};

}  // namespace vilga_detail

#endif  // VILGA_DETAIL_BACKEND_CONFIG_HPP
