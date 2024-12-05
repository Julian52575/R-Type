file(GLOB_RECURSE SRC_FILES ../game/src/*.cpp)

add_executable(Front
    ${SRC_FILES}
    resources.rc
)

find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
find_package(ASIO REQUIRED)
find_package(LUA REQUIRED)

target_link_libraries(Front PRIVATE
    asio::asio
    sfml-system
    sfml-window
    sfml-graphics
    GameEngine
    lua
)

if (WIN32)
    install(TARGETS Front DESTINATION bin)
else()
    install(TARGETS Front DESTINATION /usr/bin)
endif()

set_target_properties(Front PROPERTIES OUTPUT_NAME "R-Type")
