cmake_minimum_required(VERSION 3.20)
project(R-TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_COMPILATION_")

find_library(R-ENGINE NAMES rengine REQUIRED)
find_library(R-ENGINE-GRAPHICS NAMES rengine_graphics_sfml REQUIRED)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics audio)
find_package(ASIO REQUIRED)
find_package(LUA 5.4 REQUIRED)
find_package(nlohmann_json REQUIRED)
set(SFML_STATIC_LIBRARIES FALSE)

# Ensure that Lua is found
if (NOT LUA_FOUND)
    message(FATAL_ERROR "Lua library not found!")
else()
    message(STATUS "Lua include: ${lua_INCLUDE_DIRS}")
    message(STATUS "lua libraries: ${lua_LIBRARIES}")
endif()

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(GAME
    ${SRC_FILES}
)
target_compile_features(GAME PUBLIC cxx_std_20)

target_include_directories(GAME PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    $<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>
    ${lua_INCLUDE_DIRS}
)

target_link_libraries(GAME PUBLIC
    rengine
    rengine_graphics_sfml
    asio::asio
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    lua::lua
    nlohmann_json::nlohmann_json
)

set_target_properties(GAME PROPERTIES OUTPUT_NAME "rtype")
