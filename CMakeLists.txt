cmake_minimum_required(VERSION 3.20)
project(R-TYPE)

## Installing njoh
include(FetchContent)
#FetchContent_Declare(
#    nlohmann-json3-dev
#    URL https://github.com/nlohmann/json/releases/download/v3.11.3/include.zip
#    URL_HASH SHA256=a22461d13119ac5c78f205d3df1db13403e58ce1bb1794edc9313677313f4a9d
#)
#FetchContent_MakeAvailable(nlohmann-json3-dev)


file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(GAME
    ${SRC_FILES}
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_COMPILATION_")

target_include_directories(GAME PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    #${nlohmann-json3-dev_SOURCE_DIR}/include
    $<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>
)

find_library(R-ENGINE
    NAMES rengine REQUIRED)
set(SFML_STATIC_LIBRARIES FALSE)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics audio)
find_package(ASIO REQUIRED)
find_package(LUA REQUIRED)
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
