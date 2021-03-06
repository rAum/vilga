cmake_minimum_required(VERSION 3.10)
project(vilga VERSION 0.0.0 LANGUAGES CXX)

include(cmake/FindZeroMQ.cmake)
find_package(Threads REQUIRED)

enable_testing()

add_subdirectory(external)

add_subdirectory(vilga)

set(CPACK_PACKAGE_VENDOR "vilga")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "vilga is experimental logger for non-textual data like images, matrices, point clouds")
set(CPACK_PACKAGE_CONTACT "https://github.com/rAum/vilga/")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "https://github.com/rAum/vilga/")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_SOURCE_IGNORE_FILES
        /.git
        /.*build.*
        )
set(CPACK_STRIP_FILES 1)
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(CPACK_DEBIAN_PACKAGE_LICENSE "MIT")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/README.md)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libzmq3")

include(CPack)