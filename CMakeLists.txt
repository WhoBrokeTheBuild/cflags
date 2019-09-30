CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

PROJECT(cFlags LANGUAGES C VERSION 1.0.0)

ADD_LIBRARY(cFlags INTERFACE)

TARGET_INCLUDE_DIRECTORIES(cFlags INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

INSTALL(TARGETS cFlags
    EXPORT cFlagsTargets
    INCLUDES DESTINATION include
)

INCLUDE(CMakePackageConfigHelpers)

WRITE_BASIC_PACKAGE_VERSION_FILE(
    "${CMAKE_BINARY_DIR}/cFlagsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

INSTALL(EXPORT cFlagsTargets
    FILE cFlagsTargets.cmake
    NAMESPACE cFlags::
    DESTINATION lib/cmake/cFlags
)

CONFIGURE_PACKAGE_CONFIG_FILE(
    "${CMAKE_SOURCE_DIR}/cmake/cFlagsConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/cFlagsConfig.cmake"
    INSTALL_DESTINATION lib/cmake/cFlags
)

INSTALL(EXPORT cFlagsTargets 
    DESTINATION lib/cmake/cFlags
)

INSTALL(FILES 
    "${CMAKE_BINARY_DIR}/cFlagsConfigVersion.cmake" 
    "${CMAKE_BINARY_DIR}/cFlagsConfig.cmake"
    DESTINATION lib/cmake/cFlags
)

INCLUDE(FindPkgConfig)

IF(PKG_CONFIG_FOUND)
    CONFIGURE_FILE(
        "${CMAKE_SOURCE_DIR}/cmake/cFlags.pc.in"
        "${CMAKE_BINARY_DIR}/cFlags.pc"
        @ONLY
    )

    INSTALL(FILES
        "${CMAKE_BINARY_DIR}/cFlags.pc"
        DESTINATION share/pkgconfig
    )
ENDIF()

INSTALL(DIRECTORY
    "${CMAKE_SOURCE_DIR}/include/"
    DESTINATION include
)

# Test executable

ADD_EXECUTABLE(test source/test.c)

TARGET_LINK_LIBRARIES(test cFlags)
