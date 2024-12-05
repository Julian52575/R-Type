cmake_minimum_required(VERSION 3.20)
project(R-Engine)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE HPP_FILES src/*.hpp)

add_library(rengine SHARED
    ${SRC_FILES}
)

target_include_directories(rengine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
