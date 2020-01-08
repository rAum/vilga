# This file allows to use find_package returned doctest or the one shipped.

option(ANZU_SYSTEM_DOCTEST "Turns on/off if system installed doctest should be used instead of shipped one" OFF)

if (ANZU_SYSTEM_DOCTEST)
  find_package(doctest QUIET)
endif()

if(NOT TARGET doctest::doctest)
  if (ANZU_SYSTEM_DOCTEST)
    message(WARNING "Asked for system doctest, but it's not found")
  endif()
  message(STATUS "Adding doctest from sources")
  add_library(doctest INTERFACE)
  target_include_directories(doctest SYSTEM INTERFACE include/)
  add_library(doctest::doctest ALIAS doctest)
endif()