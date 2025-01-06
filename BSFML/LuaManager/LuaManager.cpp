#include "LuaManager.hpp"
#include <utility>
#include <iostream>
#include <vector>
#include <string>

bfs::LuaManager::LuaManager() {
}

bfs::LuaManager::~LuaManager() {
    for (auto& [name, state] : this->states) {
        lua_close(state);
    }
}

void bfs::LuaManager::addFile(const std::string& filename) {
    if (states.find(filename) != states.end()) {
        throw std::runtime_error("Lua error: file already loaded");
    }
    lua_State* L = luaL_newstate();
    if (!L)
        throw std::runtime_error("Lua error: cannot create Lua state");
    luaL_openlibs(L);
    if (luaL_dofile(L, filename.c_str()) != LUA_OK) {
        lua_pop(L, 1);
        throw std::runtime_error("Lua error: Cannot load file: " + filename);
    }
    this->states[filename] = L;
}

void bfs::LuaManager::push(lua_State *) {}

void bfs::LuaManager::push(lua_State *L,const std::string& value) {
    lua_pushstring(L, value.c_str());
}

void bfs::LuaManager::push(lua_State *L,std::string& value) {
    lua_pushstring(L, value.c_str());
}

void bfs::LuaManager::push(lua_State *L,const char* value) {
    lua_pushstring(L, value);
}

void bfs::LuaManager::push(lua_State *L,int value) {
    lua_pushinteger(L, value);
}

void bfs::LuaManager::push(lua_State *L,float value) {
    lua_pushnumber(L, value);
}

void bfs::LuaManager::push(lua_State *L,double value) {
    lua_pushnumber(L, value);
}

void bfs::LuaManager::push(lua_State *L,bool value) {
    lua_pushboolean(L, value);
}

void bfs::LuaManager::push(lua_State *L,std::nullptr_t) {
    lua_pushnil(L);
}

void bfs::LuaManager::push(lua_State *L,std::any value) {
    if (value.type() == typeid(int)) {
        push(L, std::any_cast<int>(value));
    } else if (value.type() == typeid(float)) {
        push(L, std::any_cast<float>(value));
    } else if (value.type() == typeid(double)) {
        push(L, std::any_cast<double>(value));
    } else if (value.type() == typeid(bool)) {
        push(L, std::any_cast<bool>(value));
    } else if (value.type() == typeid(std::string)) {
        push(L, std::any_cast<std::string>(value));
    } else if (value.type() == typeid(const char*)) {
        push(L, std::any_cast<const char*>(value));
    } else if (value.type() == typeid(std::nullptr_t)) {
        push(L, nullptr);
    } else if (value.type() == typeid(std::vector<std::any>)) {
        push(L, std::any_cast<std::vector<std::any>>(value));
    } else {
        throw std::runtime_error(std::string("Lua error: unsupported type: ") + std::string(typeid(value).name()) + " for std::any");
    }
}

std::unordered_map<std::string, std::any> bfs::LuaManager::processLuaTable(lua_State* L, int index) {
    std::unordered_map<std::string, std::any> result;

    lua_pushnil(L);
    while (lua_next(L, index) != 0) {
        std::string key = lua_tostring(L, -2);
        switch (lua_type(L, -1)) {
            case LUA_TNUMBER:
                result[key] = lua_tonumber(L, -1);
                break;
            case LUA_TBOOLEAN:
                result[key] = bool(lua_toboolean(L, -1));
                break;
            case LUA_TSTRING:
                result[key] = std::string(lua_tostring(L, -1));
                break;
            case LUA_TNIL:
                result[key] = nullptr;
                break;
            case LUA_TTABLE:
                result[key] = processLuaTable(L, lua_gettop(L));
                break;
            default:
                throw std::runtime_error("Lua error: unknown type");
        }
        lua_pop(L, 1);
    }
    return result;
}

std::vector<std::any> bfs::LuaManager::getLuaResult(lua_State* L) {
    int nbr_results = lua_gettop(L);
    if (nbr_results == 0)
        return {};

    std::vector<std::any> result;
    for (int i = 1; i <= nbr_results; i++) {
        switch (lua_type(L, i)) {
            case LUA_TNUMBER:
                result.push_back(lua_tonumber(L, i));
                break;
            case LUA_TBOOLEAN:
                result.push_back(bool(lua_toboolean(L, i)));
                break;
            case LUA_TSTRING:
                result.push_back(std::string(lua_tostring(L, i)));
                break;
            case LUA_TNIL:
                result.push_back(nullptr);
                break;
            case LUA_TTABLE:
                result.push_back(processLuaTable(L, i));
                break;
            default:
                throw std::runtime_error("Lua error: unknown type");
        }
    }
    lua_pop(L, nbr_results);
    return result;
}