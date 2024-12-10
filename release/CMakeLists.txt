file(GLOB_RECURSE GAME_SRC_FILES "../game/src/*.cpp")
file(GLOB_RECURSE RENGINE_SRC_FILES "../gameEngine/src/*.cpp")

find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
find_package(ASIO REQUIRED)
find_package(LUA REQUIRED)

add_executable(release
    ${GAME_SRC_FILES}
    ${RENGINE_SRC_FILES}
    resources.rc
)

target_link_libraries(release PRIVATE
    asio::asio
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    lua5.4
)

set_target_properties(release PROPERTIES OUTPUT_NAME "R-Type")
