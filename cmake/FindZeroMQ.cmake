# Try to find libzmq.
set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
find_package(PkgConfig)
pkg_check_modules(PC_LIBZMQ REQUIRED libzmq)

set(ZeroMQ_VERSION ${PC_LIBZMQ_VERSION})
find_library(ZeroMQ_LIBRARY
        NAMES zmq libzmq.so libzmq.dylib libzmq.dll
        PATHS ${PC_LIBZMQ_LIBDIR} ${PC_LIBZMQ_LIBRARY_DIRS}
)

if (TARGET libzmq)
    message(STATUS "Target libzmq already exists")
    return()
elseif(ZeroMQ_LIBRARY)
    find_path (ZMQ_INCLUDE_DIR zmq.h)
    add_library(libzmq SHARED IMPORTED GLOBAL)
    target_include_directories(libzmq SYSTEM INTERFACE ${ZMQ_INCLUDE_DIR})
    set_property(TARGET libzmq PROPERTY IMPORTED_LOCATION ${ZeroMQ_LIBRARY})
    set(ZeroMQ_FOUND ON)
endif()