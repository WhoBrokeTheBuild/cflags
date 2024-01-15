cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(
    cflags
    LANGUAGES C CXX
    DESCRIPTION "Command line flag parsing library in C/C++"
    VERSION 3.0.3
)

###
### cflags
###

add_library(cflags INTERFACE)
add_library(cflags::cflags ALIAS cflags)

target_include_directories(
    cflags INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

###
### cppflags
###

add_library(cppflags INTERFACE)
add_library(cflags::cppflags ALIAS cppflags)

target_include_directories(
    cppflags INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(
    cppflags
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

###
### Install
###

install(
    TARGETS cflags cppflags
    EXPORT cflagsTargets
    INCLUDES DESTINATION include
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cflagsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    EXPORT cflagsTargets
    NAMESPACE cflags::
    DESTINATION lib/cmake/cflags
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cflagsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cflagsConfig.cmake
    INSTALL_DESTINATION lib/cmake/cflags
)

install(
    FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/cflagsConfigVersion.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cflagsConfig.cmake
    DESTINATION lib/cmake/cflags
)

include(FindPkgConfig)

if(PKG_CONFIG_FOUND)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cflags.pc.in
        ${CMAKE_CURRENT_BINARY_DIR}/cflags.pc
        @ONLY
    )

    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/cflags.pc
        DESTINATION share/pkgconfig
    )
endif()

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include
)

###
### Test executables
###

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

    add_executable(example examples/example.c)

    target_link_libraries(example cflags)

    add_executable(example-cpp examples/example.cpp)

    target_link_libraries(example-cpp cppflags)

    set_target_properties(
        example-cpp
        PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
    )

endif()