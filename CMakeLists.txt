cmake_minimum_required(VERSION 3.20)
project(R-Type)

# Canon integration
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" CACHE FILEPATH "Conan toolchain file")

# g++ flag
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ??
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

# Call release/CMakeLists.txt to build a binary
add_subdirectory(release)

if (WIN32)
    install(TARGETS release DESTINATION bin)
else()
    install(TARGETS release DESTINATION /usr/bin)
endif()

if(WIN32)
    set(CPACK_GENERATOR "WIX")

    set(CPACK_WIX_SHORTCUTS_ON_DESKTOP ON)
    set(CPACK_WIX_CREATE_DESKTOP_SHORTCUTS "release")
    set(CPACK_WIX_PRODUCT_GUID "108415bb-0622-44d8-a270-8c22799aeb7d")

    set(CPACK_WIX_PROGRAM_ICON "${CMAKE_SOURCE_DIR}/release/assets/icon/r-type.ico")
else()
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "KK")

    install(FILES "${CMAKE_SOURCE_DIR}/release/assets/icon/r-type.png" DESTINATION /usr/share/icons)

    configure_file("${CMAKE_SOURCE_DIR}/release/assets/r-type.desktop.in" "${CMAKE_BINARY_DIR}/r-type.desktop" @ONLY)
    install(FILES "${CMAKE_BINARY_DIR}/r-type.desktop" DESTINATION /usr/share/applications)

endif()

set(CPACK_PACKAGE_NAME "R-Type")
set(CPACK_WIX_UPGRADE_GUID "108415bb-0622-44d8-a270-8c22799aeb7d")
set(CPACK_PACKAGE_VENDOR "Pokemon TCGP + échange")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "R-Type - A Networked Space Shooter")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "R-Type")

include(CPack)
