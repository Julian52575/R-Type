file(GLOB_RECURSE SRC_FILES "../game/src/*.cpp" "../gameEngine/src/*.cpp")

# Fetch lua lib
include(FetchContent)
FetchContent_Declare(
    lua_lib
    URL https://www.lua.org/ftp/lua-5.4.7.tar.gz
    URL_HASH SHA256=9fbf5e28ef86c69858f6d3d34eccc32e911c1a28b4120ff3e84aaa70cfbf1e30
)
FetchContent_MakeAvailable(lua_lib)
file(GLOB_RECURSE lua_SRC_FILES ${lua_lib_SOURCE_DIR}/src/*.c)
list(REMOVE_ITEM lua_SRC_FILES ${lua_lib_SOURCE_DIR}/src/lua.c) #removing lua main
list(REMOVE_ITEM lua_SRC_FILES ${lua_lib_SOURCE_DIR}/src/luac.c) #removing lua main
# Fetch Json lib
FetchContent_Declare(
    nlohmann-json3-dev
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/include.zip
    URL_HASH SHA256=a22461d13119ac5c78f205d3df1db13403e58ce1bb1794edc9313677313f4a9d
)
FetchContent_MakeAvailable(nlohmann-json3-dev)

# Copy game assets
file(COPY "../game/assets/" DESTINATION "./assets/")
file(COPY "../game/shaders/" DESTINATION "./assets")
file(COPY "../game/entities/" DESTINATION "./assets")
file(COPY "../game/scripts/" DESTINATION "./assets")

# Set source code
add_executable(release
    ${SRC_FILES}
    ${lua_SRC_FILES}
    resources.rc
)
# Set include
target_include_directories(release PUBLIC
    "../gameEngine/"
    "../game/src/"
    ${lua_lib_SOURCE_DIR}/src
    ${nlohmann-json3-dev_SOURCE_DIR}/include
)
# debug
message(STATUS "Executable: ${SRC_FILES}\n${lua_SRC_FILES}\n${nlohmann-json3-dev_SOURCE_DIR}/include")
# Set compile flags
target_compile_options(release PRIVATE
    "-DCMAKE_COMPILATION_"
)

find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
find_package(ASIO REQUIRED)
find_package(LUA REQUIRED)

target_link_libraries(release PRIVATE
    asio::asio
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
)

if (WIN32)
    install(TARGETS release DESTINATION bin)
else()
    install(TARGETS release DESTINATION /usr/bin)
endif()

set_target_properties(release PROPERTIES OUTPUT_NAME "R-Type")
