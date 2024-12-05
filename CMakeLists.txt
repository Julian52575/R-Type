cmake_minimum_required(VERSION 3.20)
project(R-Type)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" CACHE FILEPATH "Conan toolchain file")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

add_subdirectory(gameEngine)
add_subdirectory(game)

if (WIN32)
    install(TARGETS Front DESTINATION bin)
else()
    install(TARGETS Front DESTINATION /usr/bin)
endif()

if(WIN32)
    set(CPACK_GENERATOR "WIX")

    set(CPACK_WIX_SHORTCUTS_ON_DESKTOP ON)
    set(CPACK_WIX_CREATE_DESKTOP_SHORTCUTS "Front")
    set(CPACK_WIX_PRODUCT_GUID "108415bb-0622-44d8-a270-8c22799aeb7d")

    set(CPACK_WIX_PROGRAM_ICON "${CMAKE_SOURCE_DIR}/front/assets/icon/r-type.ico")
else()
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "KK")

    install(FILES "${CMAKE_SOURCE_DIR}/front/assets/icon/r-type.png" DESTINATION /usr/share/icons)

    configure_file("${CMAKE_SOURCE_DIR}/front/assets/r-type.desktop.in" "${CMAKE_BINARY_DIR}/r-type.desktop" @ONLY)
    install(FILES "${CMAKE_BINARY_DIR}/r-type.desktop" DESTINATION /usr/share/applications)

endif()

set(CPACK_PACKAGE_NAME "R-Type")
set(CPACK_WIX_UPGRADE_GUID "108415bb-0622-44d8-a270-8c22799aeb7d")
set(CPACK_PACKAGE_VENDOR "Pokemon TCGP + échange")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "R-Type - A Networked Space Shooter")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "R-Type")

include(CPack)
