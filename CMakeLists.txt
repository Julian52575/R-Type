cmake_minimum_required(VERSION 3.20)
project(R-TYPE)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(GAME
    ${SRC_FILES}
)

target_include_directories(GAME PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
find_package(ASIO REQUIRED)
find_package(LUA REQUIRED)

target_link_libraries(GAME PRIVATE
    asio::asio
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    lua5.4
    rengine
)

set_target_properties(GAME PROPERTIES OUTPUT_NAME "rtype")
