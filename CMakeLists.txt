cmake_minimum_required(VERSION 3.20)
project(R-Engine)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_library(rengine SHARED
    ${SRC_FILES}
)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(SFML_STATIC_LIBRARIES FALSE)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics audio)
find_package(ASIO REQUIRED)
find_package(LUA REQUIRED)

target_link_libraries(rengine PUBLIC
    asio::asio
    sfml-system 
    sfml-window
    sfml-graphics
    sfml-audio
    lua5.4
)

target_include_directories(rengine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CONAN_INCLUDE_DIRS}
)
