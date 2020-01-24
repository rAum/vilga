#pragma once
#ifndef VILGA_STD_TYPES_HPP
#define VILGA_STD_TYPES_HPP

#include <vilga/detail/named_values.hpp>

#define VILGA_STRINGIFY(X) #X
#define VILGA_TOSTRING(X) VILGA_STRINGIFY(X)
#define VILGA_FILE_LINE __FILE__ ":" VILGA_TOSTRING(__LINE__)

namespace vilga_detail {

// placeholder implementation
template<class T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
void vilga_type_cast(const T& i, vilga_detail::data& data) {
  (void) data; (void)i;
}

}  // namespace vilga_detail

namespace vilga {

//template<class T>
//using Val = vilga_detail::spell_named_value<T>;
//
//#define VILGA_VAL(V) \
//  vilga_detail::spell_named_value<decltype(V)>{VILGA_FILE_LINE , V}

} // vilga

#endif  // VILGA_STD_TYPES_HPP
