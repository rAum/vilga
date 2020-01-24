#pragma once
#ifndef VILGA_PORTAL_SPELL_HPP
#define VILGA_PORTAL_SPELL_HPP

#include <type_traits>
#include <vilga/detail/macros.hpp>

namespace vilga_detail {
class data;
}  // namespace vilga_detail

namespace vilga_detail {

/**
 * This neat class allows to send (almost) any data by portal.
 *
 * Of course portals require magic - serialization - which
 * depends on type of object. Or other trickery - compression etc.
 * For example rabbit requires a hat and a wand transformation.
 *
 * Technically it is a class to provide nice interface as
 * eat-almost-everything single type. There are two conditions:
 * - we send objects. Sending functions corrupts universe.
 * - there is provided Argument Dependant Lookup function
 *   for given type.
 *
 * Vilga provides few implementations but it should be easy to
 * expand, just remember to provide function with signature:
 * @code{.cpp}
 * void vilga_detail::vilga_type_cast(const T&, vilga_detail::data&);
 * @endcode
 *
 * Technique used is inspired by Sean Parent talk.
 * see: "Inheritance Is The Base Class of Evil"
 */
class portal_spell final {
public:
  /**
   * Implicit constructor available for objects
   * @tparam T object type
   * @param type a const reference to data
   */
  template<class T, std::enable_if_t<std::is_object<T>::value, int> = 0>
  constexpr portal_spell(const T& value) noexcept { // NOLINT
    new(&mana_source_) spell<T>(value);
    //std::cout << "constructing " << value << std::endl;
  }

  ~portal_spell() noexcept {
    auto& magic_spell = reinterpret_cast<spell_contract&>(mana_source_);
    (&magic_spell)->~spell_contract();
  }

  /**
   * Casts a spell, transforming object into vilga data.
   * @param data target of the transformation
   */
  void cast_into(vilga_detail::data& data) const {
    const auto& magic_spell = reinterpret_cast<const spell_contract&>(mana_source_);
    magic_spell.cast(data);
  }

private:
  struct spell_contract {
    virtual ~spell_contract() = default;
    virtual void cast(vilga_detail::data& out) const = 0;
  };

  template<typename T>
  struct spell : public spell_contract {
    constexpr explicit spell(const T& data) : data_(data) {}
    void cast(vilga_detail::data& out) const override {
      vilga_type_cast(data_, out);
    }
    const T& data_;
  };

  // we know that reference is type independent so we can cheat a bit ;)
  // the size would be reference + vtable with single entry.
  typename std::aligned_storage<sizeof(spell<int>), alignof(spell<int>)>::type mana_source_;
};
} // namespace vilga_detail

#endif  //VILGA_PORTAL_SPELL_HPP
