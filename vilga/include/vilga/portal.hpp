#pragma once
#ifndef VILGA_PORTAL_HPP
#define VILGA_PORTAL_HPP

#include <vilga/detail/macros.hpp>
#include <vilga/detail/backend.hpp>
#include <vilga/detail/data.hpp>
#include <vilga/detail/portal_spell.hpp>

namespace vilga VILGA_EXPORT {

/**
 * Stores some global settings for the portal
 */
struct portal_settings {
  enum class Mode {
    OFF,
    STREAM,
  } mode = Mode::OFF;
};

/**
 * Main portal class which provides interface for sending data, similar to logger.
 */
template <class Destination>
class portal {
public:
  explicit portal(portal_settings settings) : portal_settings_(settings) {
  }

  void send(const vilga_detail::portal_spell& spell) {
    if (portal_settings_.mode == portal_settings::Mode::OFF) {
      return;
    }
    just_send(spell);
  }

  void set_mode(portal_settings::Mode mode) noexcept {
    portal_settings_.mode = mode;
  }

  portal_settings::Mode get_mode() const noexcept {
    return portal_settings_.mode;
  }

private:
  void just_send(const vilga_detail::portal_spell& spell) {
    vilga_detail::data dt;
    spell.cast_into(dt);
    vilga_detail::backend_instance.consume(std::move(dt));
  }
  portal_settings portal_settings_;
};

}  // namespace vilga

#endif //  VILGA_PORTAL_HPP
