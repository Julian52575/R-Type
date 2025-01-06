#pragma once

#include <lua5.4/lua.hpp>
#include <iostream>
#include <any>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <map>

namespace bfs {
    class LuaManager {
        private:
            template <typename T, typename ...Args>
            void push(lua_State *L, T&& value, Args&&... args);

            template <typename T>
            void push(lua_State *L, T& value);

            void push(lua_State *L);
            void push(lua_State *L,const std::string& value);
            void push(lua_State *L,std::string &value);
            void push(lua_State *L,const char* value);
            void push(lua_State *L,int value);
            void push(lua_State *L,float value);
            void push(lua_State *L,double value);
            void push(lua_State *L,bool value);
            void push(lua_State *L,std::nullptr_t);
            void push(lua_State *L,std::any value);


            template <typename T, typename U>
            void push(lua_State *L,std::unordered_map<T, U> &value);
            template <typename T, typename U>
            void push(lua_State *L,std::map<T, U> &value);

            template <typename T>
            void push(lua_State *L, std::vector<T> value);


            std::vector<std::any> getLuaResult(lua_State* L);
            std::unordered_map<std::string, std::any> processLuaTable(lua_State* L, int index);

            std::unordered_map<std::string, lua_State*> states;
            
        public:
            LuaManager();
            ~LuaManager();
            void addFile(const std::string& filename);


            template <typename ...Args>
            std::vector<std::any> callFunction(const std::string& file, const std::string& name, Args... args);
    };
}

template <typename... Args>
std::vector<std::any> bfs::LuaManager::callFunction(const std::string& file, const std::string& name, Args... args) {
    if (states.find(file) == states.end())
        addFile(file);
    
    lua_State* L = states[file];

    if (lua_getglobal(L, name.c_str()) == 0)
        throw std::runtime_error("Lua error: cannot find function " + name + " in file " + file);

    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        throw std::runtime_error("Lua error: " + name + " is not a function in file " + file);
    }
    push(L,args...);
    int nbr_arg = sizeof...(args);

    if (lua_pcall(L, nbr_arg, LUA_MULTRET, 0) != LUA_OK) {
        lua_pop(L, 1);
        throw std::runtime_error("Lua error: Error while calling function " + name + " in file " + file);
    }

    return getLuaResult(L);
}

template <typename T, typename... Args>
void bfs::LuaManager::push(lua_State *L,T &&value, Args &&... args) {
    push(L,value);
    push(L,std::forward<Args>(args)...);
}


template <typename T>
void bfs::LuaManager::push(lua_State *L,std::vector<T> value) {
    lua_newtable(L);
    for (int i = 0; i < value.size(); i++) {
        push(L, value[i]);
        lua_rawseti(L, -2, i + 1);
    }
}


template <typename T>
void bfs::LuaManager::push(lua_State *L,T &value) {
    throw std::runtime_error(std::string("Lua error: unsupported type: ") + std::string(typeid(value).name()));
}

template <typename T, typename U>
void bfs::LuaManager::push(lua_State *L,std::unordered_map<T, U> &value) {
    lua_newtable(L);
    for (auto& [key, val] : value) {
        push(L, key);
        push(L, val);
        lua_settable(L, -3);
    }
}

template <typename T, typename U>
void bfs::LuaManager::push(lua_State *L,std::map<T, U> &value) {
    lua_newtable(L);
    for (auto& [key, val] : value) {
        push(L, key);
        push(L, val);
        lua_settable(L, -3);
    }
}