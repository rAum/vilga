#pragma once
#ifndef VILGA_DETAIL_DATA_HPP
#define VILGA_DETAIL_DATA_HPP

#include <memory>
#include <string>
#include <vilga/detail/macros.hpp>

namespace vilga_detail {

class memory_block_t {
public:
  std::size_t size_ = 0;
  std::unique_ptr<std::uint8_t[]> bytes_;

  static void free_mem(void* data, void*) noexcept {
    std::uint8_t* first = static_cast<std::uint8_t*>(data);
    delete[] first;
  }
};

enum class Encoding : std::uint8_t {
  VilgaExtension = 0x7F,
};

/**
 * Determines if protocol is vilga internal or external extension
 * @param protocol
 * @return true if protocol is vilga internal
 */
constexpr bool is_internal(vilga_detail::Encoding encoding) {
  constexpr std::uint8_t last_vilga_encoding =
    static_cast<std::uint8_t>(vilga_detail::Encoding::VilgaExtension);
  return static_cast<std::uint8_t>(encoding) > last_vilga_encoding;
}

class topic {
  std::string name_;
};

class header {
public:
  Encoding encoding_;
  std::array<std::uint8_t, 3> magic_bytes_;
  topic topic_;
};

/**
 * @brief Models a data frame.
 * Data frame consists header, a block of data and optional topic.
 */
class data {
public:
  header header_;
  memory_block_t data_;
};

}  // namespace vilga_detail

#endif  // VILGA_DETAIL_DATA_HPP
