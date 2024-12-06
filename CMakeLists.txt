cmake_minimum_required(VERSION 3.20)
project(R-TYPE)

## Installing njoh
include(FetchContent)
FetchContent_Declare(
    nlohmann-json3-dev
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/include.zip
    URL_HASH SHA256=a22461d13119ac5c78f205d3df1db13403e58ce1bb1794edc9313677313f4a9d
)
FetchContent_MakeAvailable(nlohmann-json3-dev)


file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(GAME
    ${SRC_FILES}
)

target_include_directories(GAME PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${nlohmann-json3-dev_SOURCE_DIR}/include
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
