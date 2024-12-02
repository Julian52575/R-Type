#include "src/Component/Game/Scripting/Scripting.hpp"
#include <utility>
#include <iostream>
#include <vector>
#include <string>

Scripting::Scripting() {
    L = luaL_newstate();
    if (!L) {
        std::cerr << "Lua error: cannot create Lua state" << std::endl;
        return;
    }
    luaL_openlibs(L);
}

Scripting::Scripting(const std::string& filename) : Scripting() {
    executeScript(filename);
}

Scripting::~Scripting() {
    lua_close(L);
}

void Scripting::executeScript(const std::string& filename) {
    if (luaL_dofile(L, filename.c_str())) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}

template <typename T>
void Scripting::push(T &&value){
    if constexpr (std::is_same<std::decay_t<T>, std::string>::value)
        lua_pushstring(L, value.c_str());
    else if constexpr (std::is_same<std::decay_t<T>, int>::value)
        lua_pushinteger(L, value);
    else if constexpr (std::is_same<std::decay_t<T>, float>::value)
        lua_pushnumber(L, value);
    else if constexpr (std::is_same<std::decay_t<T>, double>::value)
        lua_pushnumber(L, value);
    else if constexpr (std::is_same<std::decay_t<T>, bool>::value)
        lua_pushboolean(L, value);
    else if constexpr (std::is_same<std::decay_t<T>, std::nullptr_t>::value)
        lua_pushnil(L);
}

template <typename T, typename... Args>
void Scripting::push(T &&value, Args &&... args) {
    push(value);
    push(std::forward<Args>(args)...);
}

void Scripting::push() {}

template <typename... Args>
std::vector<std::any> Scripting::callFunction(std::string name, Args... args) {
    if(!lua_getglobal(L, name.c_str())){
        std::cerr << "Lua error: cannot find function " << name << std::endl;
        return {};
    }

    if (!lua_isfunction(L, -1)) {
        std::cerr << "Lua error: " << name << " is not a function" << std::endl;
        lua_pop(L, 1);
        return {};
    }

    push(args...);
    int nbr_arg = sizeof...(args);

    if (lua_pcall(L, nbr_arg, LUA_MULTRET, 0)) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return {};
    }

    std::vector<std::any> results;
    int nbr_results = lua_gettop(L);
    if (nbr_results == 0) {
        lua_pop(L, 1);
        return results;
    }

    for (int i = 1; i <= nbr_results; i++) {
        if (lua_isnumber(L, i)) {
            double result = lua_tonumber(L, i);
            results.push_back(result);
        } else if (lua_isstring(L, i)) {
            std::string result = lua_tostring(L, i);
            results.push_back(result);
        } else if (lua_isboolean(L, i)) {
            bool result = lua_toboolean(L, i);
            results.push_back(result);
        }
    }

    lua_pop(L, nbr_results);
    return results;
}

