cmake_minimum_required(VERSION 3.20)
project(R-Engine)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_library(GameEngine STATIC
    ${SRC_FILES}
)

target_include_directories(GameEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
