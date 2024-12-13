file(GLOB_RECURSE SRC_FILES "../game/src/*.cpp" "../rengine/src/*.cpp")

# Set source code
add_executable(release
    ${SRC_FILES}
    resources.rc
)

# Set dependencies
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
find_package(ASIO REQUIRED)
find_package(LUA REQUIRED)
find_package(Lua REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(release PRIVATE
    asio::asio
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    lua::lua
    nlohmann_json::nlohmann_json
)

# Set include
target_include_directories(release PUBLIC
    "../rengine/"
    "../game/src/"
    "../game/"
    $<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>
    ".."
)

# Set compile flags
target_compile_options(release PRIVATE
    "-DCMAKE_COMPILATION_"
)

if (WIN32)
    install(TARGETS release DESTINATION bin)
else()
    install(TARGETS release DESTINATION /usr/bin)
endif()

set_target_properties(release PROPERTIES OUTPUT_NAME "R-Type")
