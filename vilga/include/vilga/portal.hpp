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
    vilga_detail::data dt;
    spell.cast_into(dt);
    backend_.consume(std::move(dt));
  }

private:
  portal_settings portal_settings_;
  vilga_detail::backend backend_;
};

}  // namespace vilga

#endif //  VILGA_PORTAL_HPP
