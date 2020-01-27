#pragma once
#ifndef VILGA_DETAIL_NAMED_VALUE_HPP
#define VILGA_DETAIL_NAMED_VALUE_HPP

#include <vilga/detail/macros.hpp>
#include <vilga/detail/portal_spell.hpp>
#include <type_traits>
#include <utility>

namespace vilga_detail {

/**
 * Implements support for basic arithmetic types for named values
 * @tparam T serializable arithmetic type
 */
template<class T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class VILGA_EXPORT spell_named_value : public portal_spell {
public:
  spell_named_value(std::string name, const T& object) noexcept : ref_(object), name_(std::move(name)) {
  }

  void cast_into(data& data) const noexcept override {
    (void)data;
  }

private:
  std::reference_wrapper<const T> ref_;
  std::string name_;
};

}  // vilga_detail

#endif  // VILGA_DETAIL_NAMED_VALUE_HPP
