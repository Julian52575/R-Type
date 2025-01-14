cmake_minimum_required(VERSION 3.20)
set(PROJECT_VERSION 1.0.0)
project(R-Type VERSION ${PROJECT_VERSION})

# Canon integration
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" CACHE FILEPATH "Conan toolchain file")

# g++ flag
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Call release/CMakeLists.txt to build a binary
# ---------------------------------------------
#add_subdirectory(release)
file(GLOB_RECURSE GAME_FILES "game/src/*.cpp")
file(GLOB_RECURSE RENGINE_FILES "rengine/src/*.cpp")
list(REMOVE_ITEM RENGINE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/rengine/src/main.cpp) #removing main

message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Compiling game:\n${GAME_FILES}")
message(STATUS "Compiling rengine:\n${RENGINE_FILES}")

# Set source code
add_executable(release
    ${RENGINE_FILES}
    ${GAME_FILES}
    release/resources.rc
)

# Set dependencies
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics audio)
find_package(ASIO REQUIRED)
find_package(LUA REQUIRED)
find_package(nlohmann_json REQUIRED)

target_link_libraries(release PUBLIC
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    asio::asio
    lua::lua
    nlohmann_json::nlohmann_json
    # ws2_32
)

message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")

# Set include
target_include_directories(release PUBLIC
    "rengine/"
    "game/src/"
    "game/"
    $<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${nlohmann_json_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIR}
    ${asio_INCLUDE_DIR}
)

# Set compile flags
target_compile_options(release PUBLIC
    "-DCMAKE_COMPILATION_"
    "-D__RENGINEGRAPHICS_USELIB_SFML"
)

if (WIN32)
    install(TARGETS release DESTINATION bin)
else()
    install(TARGETS release DESTINATION /usr/bin)
endif()

set_target_properties(release PROPERTIES OUTPUT_NAME "R-Type")
# ---------------------------------------------

# Define OS Specifics
if (WIN32)
    ## ??
    add_definitions(-D_WIN32_WINNT=0x0A00)
    # Install dir
    set(INSTALL_ROOT_DIR bin)
    # CPack exclusive 
    set(CPACK_GENERATOR "WIX")
    set(CPACK_WIX_VERSION 4)
    set(CPACK_WIX_SHORTCUTS_ON_DESKTOP ON)
    set(CPACK_WIX_CREATE_DESKTOP_SHORTCUTS "release")
    #set(CPACK_WIX_PRODUCT_GUID "108415bb-0622-44d8-a270-8c22799aeb7d")
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
    # Dependencies
    #set(CPACK_DEBIAN_PACKAGE_DEPENDS "SFML lua")

endif()

# Set installing rules
install(TARGETS release DESTINATION ${INSTALL_ROOT_DIR})
install(DIRECTORY "game/assets" DESTINATION "${INSTALL_ROOT_DIR}") 
install(DIRECTORY "release/assets/icon" DESTINATION "${INSTALL_ROOT_DIR}/assets")
install(FILES "release/assets/r-type.desktop.in" DESTINATION "${INSTALL_ROOT_DIR}/assets")
install(FILES "game/serverConfig.json" DESTINATION "${INSTALL_ROOT_DIR}")


# Set Cpack variables
set(CPACK_PACKAGE_NAME "R-Type")
set(CPACK_WIX_UPGRADE_GUID "108415bb-0622-44d8-a270-8c22799aeb7d")
set(CPACK_PACKAGE_VENDOR "Pokemon TCGP + échange")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "R-Type - A Networked Space Shooter")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "R-Type")

include(CPack)
