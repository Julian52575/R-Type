file(GLOB_RECURSE SRC_FILES "../game/src/*.cpp" "../gameEngine/src/*.cpp")

# Copy game assets
file(COPY "../game/assets/" DESTINATION "./assets/")
file(COPY "../game/shaders/" DESTINATION "./assets")
file(COPY "../game/entities/" DESTINATION "./assets")
file(COPY "../game/scripts/" DESTINATION "./assets")

add_executable(releaseAssets
    ${SRC_FILES}
    resources.rc
)

find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
find_package(ASIO REQUIRED)
find_package(LUA REQUIRED)

target_link_libraries(releaseAssets PRIVATE
    asio::asio
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    lua5.4
)

if (WIN32)
    install(TARGETS releaseAssets DESTINATION bin)
else()
    install(TARGETS releaseAssets DESTINATION /usr/bin)
endif()

set_target_properties(releaseAssets PROPERTIES OUTPUT_NAME "R-Type")
