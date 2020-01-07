
add_library(vilga
        src/vilga.cpp
)

target_compile_features(vilga INTERFACE cxx_std_11)
target_compile_features(vilga PRIVATE cxx_std_14)

add_library(vilga::vilga ALIAS vilga)

target_include_directories(vilga PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
)

target_include_directories(vilga PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/>)

set_target_properties(vilga PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN 1)

install(TARGETS vilga
        EXPORT vilga-export
        COMPONENT vilga-dev)

install(DIRECTORY include/
        DESTINATION include/
        COMPONENT vilga-dev)

install(EXPORT vilga-export
        FILE vilgaConfig.cmake
        DESTINATION share/vilga
        COMPONENT vilga-dev)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/vilgaConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)
install(FILES
        "${PROJECT_BINARY_DIR}/vilgaConfigVersion.cmake"
        DESTINATION share/vilga
        COMPONENT vilga-dev)

export(TARGETS vilga FILE vilgaTargets.cmake)
export(PACKAGE vilga)
