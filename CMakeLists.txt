# Declare project, source file
cmake_minimum_required(VERSION 3.20)
project(R-Engine)
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp) #removing main
message(STATUS "Removed ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp from compilation")
message(STATUS "Compiling ${SRC_FILES}...")

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_COMPILATION_")
# Use SFML
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__RENGINEGRAPHICS_USELIB_SFML")

# Conan integration ?
# list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
include(${CMAKE_TOOLCHAIN_FILE})
message (STATUS "Toolchain: ${CMAKE_TOOLCHAIN_FILE}")

# Install libs
set(SFML_STATIC_LIBRARIES FALSE)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics audio)
find_package(asio REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Lua REQUIRED)

# Define the target
add_library(rengine SHARED ${SRC_FILES})

# Link libs
target_link_libraries(rengine PUBLIC
    asio::asio
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    nlohmann_json::nlohmann_json
    lua::lua
)

# Include headers
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
target_include_directories(rengine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SFML_INCLUDE_DIR}
    ${asio_INCLUDE_DIR}
    $<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>
)

if (WIN32)
    message(STATUS "Configuring for Windows")
endif (WIN32)
if (UNIX)
    message(STATUS "Configuring for Unix")
endif (UNIX)
