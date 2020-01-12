# This file allows to use find_package returned doctest or the one shipped.

option(VILGA_SYSTEM_CEREAL "Turns on/off if system installed doctest should be used instead of shipped one" OFF)

if (VILGA_SYSTEM_CEREAL)
    find_package(cereal QUIET)
endif()

if(NOT TARGET cereal)
    if (VILGA_SYSTEM_CEREAL)
        message(WARNING "Asked for system cereal library, but it's not found!")
    endif()
    message(STATUS "Adding cereal from sources")
    # this library is header-only, we don't want to export it so we need to use IMPORTED GLOBAL.
    add_library(cereal INTERFACE IMPORTED GLOBAL)
    target_include_directories(cereal SYSTEM INTERFACE include/)
endif()