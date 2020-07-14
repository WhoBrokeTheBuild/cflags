CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

PROJECT(cflags
    LANGUAGES C
    DESCRIPTION "Command line flag parsing library in C"
    VERSION 2.0.1)

###
### Install
###

ADD_LIBRARY(cflags INTERFACE)

TARGET_INCLUDE_DIRECTORIES(
    cflags INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

INSTALL(
    TARGETS cflags
    EXPORT cflagsTargets
    INCLUDES DESTINATION include
)

INCLUDE(CMakePackageConfigHelpers)

WRITE_BASIC_PACKAGE_VERSION_FILE(
    ${CMAKE_BINARY_DIR}/cflagsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

INSTALL(
    EXPORT cflagsTargets
    NAMESPACE cflags::
    DESTINATION lib/cmake/cflags
)

CONFIGURE_PACKAGE_CONFIG_FILE(
    ${CMAKE_SOURCE_DIR}/cmake/cflagsConfig.cmake.in
    ${CMAKE_BINARY_DIR}/cflagsConfig.cmake
    INSTALL_DESTINATION lib/cmake/cflags
)

INSTALL(
    FILES 
        ${CMAKE_BINARY_DIR}/cflagsConfigVersion.cmake
        ${CMAKE_BINARY_DIR}/cflagsConfig.cmake
    DESTINATION lib/cmake/cflags
)

INCLUDE(FindPkgConfig)

IF(PKG_CONFIG_FOUND)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/cmake/cflags.pc.in
        ${CMAKE_BINARY_DIR}/cflags.pc
        @ONLY
    )

    INSTALL(
        FILES ${CMAKE_BINARY_DIR}/cflags.pc
        DESTINATION share/pkgconfig
    )
ENDIF()

INSTALL(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
)
