file(GLOB_RECURSE GAME_FILES "../game/src/*.cpp")
file(GLOB_RECURSE RENGINE_FILES "../rengine/src/*.cpp")
list(REMOVE_ITEM RENGINE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../rengine/src/main.cpp) #removing main

message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Compiling game:\n${GAME_FILES}")
message(STATUS "Compiling rengine:\n${RENGINE_FILES}")

# Set source code
add_executable(release
    ${GAME_FILES}
    ${RENGINE_FILES}
    resources.rc
)

# Set dependencies
# find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics audio)
# find_package(ASIO REQUIRED)
# find_package(LUA REQUIRED)
# find_package(nlohmann_json REQUIRED)

target_link_libraries(release PRIVATE
    asio::asio
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    lua::lua
    nlohmann_json::nlohmann_json
    ws2_32
)

# Set include
target_include_directories(release PUBLIC
    "../rengine/"
    "../game/src/"
    "../game/"
    $<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>
    ".."
    ${nlohmann_json_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIR}
    ${asio_INCLUDE_DIR}
)

# Set compile flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(release PRIVATE
        "-DCMAKE_COMPILATION_"
        "-D__RENGINEGRAPHICS_USELIB_SFML"  # Add the flag for g++
    )
else()
    target_compile_options(release PRIVATE
        "-DCMAKE_COMPILATION_"
        "-D__RENGINEGRAPHICS_USELIB_SFML"  # Add the flag for other compilers as well
)
endif()

if (WIN32)
    install(TARGETS release DESTINATION bin)
else()
    install(TARGETS release DESTINATION /usr/bin)
endif()

set_target_properties(release PROPERTIES OUTPUT_NAME "R-Type")
