cmake_minimum_required(VERSION 3.20)
include(FetchContent)
# Declare project, source file
project(R-Engine)
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Get lua lib on this cmake, conan would never
FetchContent_Declare(
    lua_lib
    URL https://www.lua.org/ftp/lua-5.4.7.tar.gz
    URL_HASH SHA256=9fbf5e28ef86c69858f6d3d34eccc32e911c1a28b4120ff3e84aaa70cfbf1e30
)
FetchContent_MakeAvailable(lua_lib)
file(GLOB_RECURSE lua_SRC_FILES ${lua_lib_SOURCE_DIR}/src/*.c)
list(REMOVE_ITEM lua_SRC_FILES ${lua_lib_SOURCE_DIR}/src/lua.c) #removing lua main

message(STATUS "Compiling ${SRC_FILES} && ${lua_SRC_FILES}")
add_library(rengine SHARED
    ${SRC_FILES}
    ${lua_SRC_FILES}
)
target_include_directories(rengine PUBLIC ${lua_lib_SOURCE_DIR}/src)

# Set Compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Conan integration ?
# list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
include(${CMAKE_TOOLCHAIN_FILE})
message (STATUS "Toolchain: ${CMAKE_TOOLCHAIN_FILE}")

# Install libs
set(SFML_STATIC_LIBRARIES FALSE)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics audio)
find_package(asio REQUIRED)

# Get json lib
FetchContent_Declare(
    nlohmann-json3-dev
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/include.zip
    URL_HASH SHA256=a22461d13119ac5c78f205d3df1db13403e58ce1bb1794edc9313677313f4a9d
)
FetchContent_MakeAvailable(nlohmann-json3-dev)


# Link libs
target_link_libraries(rengine PUBLIC
    asio::asio
    sfml-system 
    sfml-window
    sfml-graphics
    sfml-audio
)

# Include headers
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
target_include_directories(rengine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SFML_INCLUDE_DIR}
    ${asio_INCLUDE_DIR}
    ${nlohmann-json3-dev_SOURCE_DIR}/include
)

if (WIN32)
    message(STATUS "Configuring for Windows")
endif (WIN32)
if (UNIX)
    message(STATUS "Configuring for Unix")
endif (UNIX)
