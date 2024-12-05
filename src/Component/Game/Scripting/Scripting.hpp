#pragma once
#include <lua/lua.hpp>
#include <iostream>
#include <any>
#include <vector>
#include <string>

class Scripting {
    private:
        lua_State* L;
    public:
        Scripting();
        explicit Scripting(const std::string&);
        ~Scripting();
        void executeScript(const std::string& filename);
        template <typename T>
        void push(T&& value);
        template <typename T, typename ...Args>
        void push(T&& value, Args&&... args);
        void push();
        template <typename ...Args>
        std::vector<std::any> callFunction(std::string name, Args... args);
};
