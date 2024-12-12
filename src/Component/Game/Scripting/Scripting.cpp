#include "./Scripting.hpp"
#include <utility>
#include <iostream>
#include <vector>
#include <string>

Scripting::Scripting() {
    L = std::shared_ptr<lua_State>(luaL_newstate(), [](lua_State* L){ lua_close(L); });
    if (!L) {
        std::cerr << "Lua error: cannot create Lua state" << std::endl;
        return;
    }
    luaL_openlibs(L.get());
}

Scripting::Scripting(const std::string& filename) : Scripting() {
    LoadScript(filename);
}

Scripting::~Scripting() {}

void Scripting::LoadScript(const std::string& filename) {
    lua_settop(L.get(), 0);

    if (luaL_dofile(L.get(), filename.c_str()) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L.get(), -1) << std::endl;
        lua_pop(L.get(), 1);
    }
}

void Scripting::push() {}

void Scripting::push(const std::string& value) {
    lua_pushstring(L.get(), value.c_str());
}

void Scripting::push(int value) {
    lua_pushinteger(L.get(), value);
}

void Scripting::push(float value) {
    lua_pushnumber(L.get(), value);
}

void Scripting::push(double value) {
    lua_pushnumber(L.get(), value);
}

void Scripting::push(bool value) {
    lua_pushboolean(L.get(), value);
}

void Scripting::push(std::nullptr_t) {
    lua_pushnil(L.get());
}



