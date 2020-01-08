#pragma once
#ifndef VILGA_VILGA_H
#define VILGA_VILGA_H

namespace vilga {

class header {

};

class datablob {

};

class data {
  header   header;
  datablob data;
};

struct logger_setup {
  bool is_enabled;
};

/**
 * Main logger class which provides interface for sending data
 */
class logger {
public:

  logger(logger_setup) {

  }

  template<class ...Args>
  void log(const Args &... args) {
    if (!setup_.is_enabled) return;
    {
      memory_buf_t buf;
      fmt::format_to(buf, fmt, args...);
      details::log_msg log_msg(loc, name_, lvl, string_view_t(buf.data(), buf.size()));
      log_it_(log_msg, log_enabled, traceback_enabled);
    }
  }

private:
  logger_setup setup_;
};

}  // namespace vilga

#endif //  VILGA_VILGA_H
