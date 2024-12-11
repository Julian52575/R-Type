#pragma once
#ifdef CMAKE_COMPILATION_
    #include <lua.hpp>
#else
    #include <lua5.4/lua.hpp>
#endif
#include <iostream>
#include <any>
#include <vector>
#include <string>
#include <memory>

class Scripting {
    private:
        std::shared_ptr<lua_State> L;
        template <typename T, typename ...Args>
        void push(T&& value, Args&&... args);
        void push();

        void push(const std::string& value);
        void push(int value);
        void push(float value);
        void push(double value);
        void push(bool value);
        void push(std::nullptr_t);
        
    public:
        Scripting();
        void LoadScript(const std::string& filename);
        explicit Scripting(const std::string&);
        ~Scripting();

        template <typename ...Args>
        std::vector<std::any> callFunction(std::string name, Args... args);
};

template <typename... Args>
std::vector<std::any> Scripting::callFunction(std::string name, Args... args) {
    if (lua_getglobal(L.get(), name.c_str()) == 0) {
        std::cerr << "Lua error: cannot find function " << name << std::endl;
        return {};
    }

    if (!lua_isfunction(L.get(), -1)) {
        std::cerr << "Lua error: " << name << " is not a function" << std::endl;
        lua_pop(L.get(), 1);
        return {};
    }
    push(args...);
    int nbr_arg = sizeof...(args);

    if (lua_pcall(L.get(), nbr_arg, LUA_MULTRET, 0) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L.get(), -1) << std::endl;
        lua_pop(L.get(), 1);
        return {};
    }

    int nbr_results = lua_gettop(L.get());
    if (nbr_results == 0) 
        return {};

    std::vector<std::any> results;
    for (int i = 1; i <= nbr_results; i++) {
        if (lua_isnumber(L.get(), i)) {
            double result = lua_tonumber(L.get(), i);
            results.push_back(result);
        } else if (lua_isstring(L.get(), i)) {
            std::string result = lua_tostring(L.get(), i);
            results.push_back(result);
        } else if (lua_isboolean(L.get(), i)) {
            bool result = lua_toboolean(L.get(), i);
            results.push_back(result);
        }
    }

    lua_pop(L.get(), nbr_results);
    return results;
}

template <typename T, typename... Args>
void Scripting::push(T &&value, Args &&... args) {
    push(value);
    push(std::forward<Args>(args)...);
}