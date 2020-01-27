#pragma once
#ifndef VILGA_PORTAL_SPELL_HPP
#define VILGA_PORTAL_SPELL_HPP

#include <vilga/detail/macros.hpp>

namespace vilga {
  template<class T>
  class portal;
}

namespace vilga_detail {

class data;

/**
 * This class is an interface which allows to send any data by portal
 */
class VILGA_EXPORT portal_spell {
public:
  template<class>
  friend class vilga::portal;

  virtual ~portal_spell() = default;
private:
  // probably it shouldn't be no except.
  virtual void cast_into(vilga_detail::data& data) const noexcept = 0;
};
} // namespace vilga_detail

#endif  //VILGA_PORTAL_SPELL_HPP
