cmake_minimum_required(VERSION 3.20)
set(PROJECT_VERSION 1.0.0)
project(R-Type VERSION ${PROJECT_VERSION})

# Canon integration
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" CACHE FILEPATH "Conan toolchain file")

# g++ flag
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
find_package(ASIO REQUIRED)
find_package(LUA REQUIRED)

# Call release/CMakeLists.txt to build a binary
add_subdirectory(release)

# Define OS Specifics
if (WIN32)
    ## ??
    add_definitions(-D_WIN32_WINNT=0x0A00)
    # Install dir
    set(INSTALL_ROOT_DIR bin)
    # CPack exclusive 
    set(CPACK_GENERATOR "WIX")
    set(CPACK_WIX_SHORTCUTS_ON_DESKTOP ON)
    set(CPACK_WIX_CREATE_DESKTOP_SHORTCUTS "release")
    set(CPACK_WIX_PRODUCT_GUID "108415bb-0622-44d8-a270-8c22799aeb7d")
    set(CPACK_WIX_PROGRAM_ICON "${CMAKE_SOURCE_DIR}/release/assets/icon/r-type.ico")
else()
    # Install dir
    set(INSTALL_ROOT_DIR "/usr/games/R-Type")
    # CPAck 
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "KK")
    # Desktop icon
    install(FILES "${CMAKE_SOURCE_DIR}/release/assets/icon/r-type.png" DESTINATION /usr/share/icons)
    configure_file("${CMAKE_SOURCE_DIR}/release/assets/r-type.desktop.in" "${CMAKE_BINARY_DIR}/r-type.desktop" @ONLY)
    install(FILES "${CMAKE_BINARY_DIR}/r-type.desktop" DESTINATION /usr/share/applications)
    execute_process(COMMAND ln -s /usr/bin/R-Type /usr/local/games/R-Type)
    # Dependencies
    #set(CPACK_DEBIAN_PACKAGE_DEPENDS "SFML lua")

endif()

# Set installing rules
install(TARGETS release DESTINATION ${INSTALL_ROOT_DIR})
install(DIRECTORY "game/assets" DESTINATION "${INSTALL_ROOT_DIR}") 
install(DIRECTORY "release/assets/icon" DESTINATION "${INSTALL_ROOT_DIR}/assets")
install(FILES "release/assets/r-type.desktop.in" DESTINATION "${INSTALL_ROOT_DIR}/assets")

# Set Cpack variables
set(CPACK_PACKAGE_NAME "R-Type")
set(CPACK_WIX_UPGRADE_GUID "108415bb-0622-44d8-a270-8c22799aeb7d")
set(CPACK_PACKAGE_VENDOR "Pokemon TCGP + échange")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "R-Type - A Networked Space Shooter")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "R-Type")

include(CPack)

if (WIN32)
    message(STATUS "NO SYMLINK")
else()
    #install(release "
    #execute_process(
    #    COMMAND sudo ln -sf /usr/bin ${INSTALL_ROOT_DIR}/R-Type
    #)
    #") #No right
endif()
