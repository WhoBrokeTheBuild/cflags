CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

PROJECT(
    cflags
    LANGUAGES C CXX
    DESCRIPTION "Command line flag parsing library in C/C++"
    VERSION 3.0.0
)

###
### cflags
###

ADD_LIBRARY(cflags INTERFACE)

TARGET_INCLUDE_DIRECTORIES(
    cflags INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

###
### cppflags
###

ADD_LIBRARY(cppflags INTERFACE)

TARGET_INCLUDE_DIRECTORIES(
    cppflags INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

SET_TARGET_PROPERTIES(
    cppflags
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

###
### Install
###

INSTALL(
    TARGETS cflags cppflags
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

###
### Test executables
###

ADD_EXECUTABLE(example examples/example.c)

TARGET_LINK_LIBRARIES(example cflags)

ADD_EXECUTABLE(example-cpp examples/example.cpp)

TARGET_LINK_LIBRARIES(example-cpp cppflags)

SET_TARGET_PROPERTIES(
    example-cpp
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
