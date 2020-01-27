#pragma once
#ifndef VILGA_DETAIL_MACROS_HPP
#define VILGA_DETAIL_MACROS_HPP

// This file contains minimal, required suite of macros for API tagging in more cross-compile manner

#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
  #ifdef vilga_EXPORTS // FIXME: make this work
      #define VILGA_EXPORT __declspec(dllexport)
   #else
      #define VILGA_EXPORT __declspec(dllimport)
  #endif
#elif defined __GNUC__ && __GNUC__ >= 4
  #define VILGA_EXPORT __attribute__((visibility("default")))
#else
  #define VILGA_EXPORT
#endif

#ifdef __GNUC__
  #define VILGA_DEPRECATED(func) func __attribute__((deprecated))
#elif defined(_MSC_VER)
  #define VILGA_DEPRECATED(func) __declspec(deprecated) func
#else
  #pragma message("WARNING: You need to implement DEPRECATED for this compiler")
  #define VILGA_DEPRECATED(func) func
#endif

#endif  // VILGA_DETAIL_MACROS_HPP
