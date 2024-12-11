cmake_minimum_required(VERSION 3.20)
project(R-TYPE)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(GAME
    ${SRC_FILES}
)

target_include_directories(GAME PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    $<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>
)

find_library(R-ENGINE
    NAMES rengine REQUIRED)
set(SFML_STATIC_LIBRARIES FALSE)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics audio)
find_package(ASIO REQUIRED)
find_package(Lua REQUIRED)
find_package(nlohmann_json REQUIRED)

target_link_libraries(GAME PUBLIC
    rengine
    asio::asio
    sfml-system 
    sfml-window
    sfml-graphics
    sfml-audio
    lua::lua
    nlohmann_json::nlohmann_json
)

set_target_properties(GAME PROPERTIES OUTPUT_NAME "rtype")
