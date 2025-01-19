//
#ifndef SRC_GAME_LUAMANAGER_HPP_
#define SRC_GAME_LUAMANAGER_HPP_

#if _WIN32
    #include <lua.hpp>
#else
    #include <lua5.4/lua.hpp>
#endif

#include <iostream>
#include <any>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <map>
#include <optional>
#include <queue>

namespace RType {

    class LuaManagerException : public std::exception {
        public:
            explicit LuaManagerException(const std::string& msg)
            {
                this->_msg = "RType::LuaManager: " + msg;
            }
            ~LuaManagerException(void) = default;
            const char *what() const noexcept {
                return this->_msg.c_str();
            }

        private:
            std::string _msg;
    };

    enum LuaType {
        LuaTypeNA,   // LUA_TNIL
        LuaTypeInt,  // LUA_TNUMBER
        LuaTypeBoolean,  // LUA_TBOOLEAN
        LuaTypeString,  // LUA_TSTRING
        LuaTypeTable,   // LUA_TTable
        LuaTypeFunction,  // LUA_TFUNCTION
        LuaTypeUserData,  // LUA_TUSERDATA
        LuaTypeThread,  // LUA_TTHREAD
        LuaTypeLightUserData  // LUA_TLIGHTUSERDATA
    };
    LuaType luaConstantToLuaType(int lua_type_);
    union LuaData {
        int64_t integer;
        const char *str;
        bool boolean;
        lua_State* luaThread;
        void *userData;
    };
    struct LuaReturn {
        LuaType type = LuaTypeNA;
        LuaData data = {0};
    };

    /**
    * @addtogroup RType
    * @namespace RType
    * @class LuaManager
    * @brief Manage multiple lua scripts
    */
    class LuaManager {
        public:
            LuaManager(void);
            ~LuaManager(void);
            /**
            * @fn addScript
            * @param The script's id.
            * @brief Load a script into the manager and return the id of the lua state.
            */
            int loadLuaScript(const std::string& filename);
            /**
            * @fn callFunction
            * @template Args The arguments to pass to the function
            * @param filename The script to run.
            * @param id The script id returned by this->loadLuaScript().
            * @param functionName The lua function from the script to run.
            * @return std::vector<std::any> A vector containing the result from the lua script. Using lua types.
            * @brief Call the lua function 'name' from the script 'filename' with the provided args.
            */
            template <typename ...Args>
            std::vector<LuaReturn> callFunction(const std::string& filename, int id, const std::string& functionName, Args... args);

            void removeScriptAtIndex(const std::string& filename, int id);

        private:
            template <typename T, typename ...Args>
            void push(lua_State *L, T&& value, Args&&... args);

            template <typename T>
            void push(lua_State *L, T& value);

            void push(lua_State *L);
            void push(lua_State *L, const std::string& value);
            void push(lua_State *L, const char* value);
            void push(lua_State *L, int value);
            void push(lua_State *L, float value);
            void push(lua_State *L, double value);
            void push(lua_State *L, bool value);
            void push(lua_State *L, std::nullptr_t);
            void push(lua_State *L, const std::any& value);

            template <typename T, typename U>
            void push(lua_State *L,std::unordered_map<T, U> &value);
            template <typename T, typename U>
            void push(lua_State *L,std::map<T, U> &value);

            template <typename T>
            void push(lua_State *L, std::vector<T> value);

            std::vector<LuaReturn> getLuaResult(lua_State* L);
            /**
             * @fn processLuaTable
             * @return The number of elements removed from the stack.
             * @brief Parse a table from the stack.
             */
            int processLuaTable(lua_State* L, int index, std::vector<LuaReturn>& result);

            std::unordered_map<std::string, std::vector<lua_State*>> states;
            std::unordered_map<std::string, std::queue<int>> freeIds;
    };

    // Doesn't compile outside of hpp
    template <typename... Args>
    std::vector<LuaReturn> LuaManager::callFunction(const std::string& file, int id, const std::string& name, Args... args)
    {
        if (states.find(file) == states.end()) {
            //throw LuaManagerException("Lua error: file " + file +" needs to be loaded before calling a function");
            id = this->loadLuaScript(file);
        }
        std::vector<lua_State*> state = this->states[file];
        if (state.size() <= id || id < 0) {
            throw LuaManagerException("Lua error: id " + std::to_string(id) + " not found in file " + file);
        }

        lua_State* L = state[id];
        if (L == nullptr) {
            throw LuaManagerException("Lua error: id " + std::to_string(id) + " is not loaded in file " + file);
        }

        if (lua_getglobal(L, name.c_str()) == 0) {
            throw LuaManagerException("Lua error: cannot find function " + name + " in file " + file);
        }
        if (!lua_isfunction(L, -1)) {
            lua_pop(L, 1);
            throw LuaManagerException("Lua error: " + name + " is not a function in file " + file);
        }
        push(L, args...);
        int nbr_arg = sizeof...(args);

        if (lua_pcall(L, nbr_arg, LUA_MULTRET, 0) != LUA_OK) {
            lua_pop(L, 1);
            throw LuaManagerException("Lua error: Error while calling function " + name + " in file " + file);
        }
        return this->getLuaResult(L);
    }

    template <typename T, typename... Args>
    void LuaManager::push(lua_State *L,T &&value, Args &&... args)
    {
        push(L,value);
        push(L,std::forward<Args>(args)...);
    }

    template <typename T>
    void LuaManager::push(lua_State *L,std::vector<T> value)
    {
        lua_newtable(L);
        for (int i = 0; i < value.size(); i++) {
            push(L, value[i]);
            lua_rawseti(L, -2, i + 1);
        }
    }

    template <typename T>
    void LuaManager::push(lua_State *L, T &value)
    {
        throw LuaManagerException(std::string("Lua error: unsupported type: ") + std::string(typeid(value).name()));
    }

    template <typename T, typename U>
    void LuaManager::push(lua_State *L,std::unordered_map<T, U> &value)
    {
        lua_newtable(L);
        for (auto& [key, val] : value) {
            push(L, key);
            push(L, val);
            lua_settable(L, -3);
        }
    }

    template <typename T, typename U>
    void LuaManager::push(lua_State *L, std::map<T, U> &value)
    {
        lua_newtable(L);
        for (auto& [key, val] : value) {
            push(L, key);
            push(L, val);
            lua_settable(L, -3);
        }
    }

    class LuaManagerSingletone {
        public:
            LuaManagerSingletone(void) = default;
            ~LuaManagerSingletone(void) = default;

            static LuaManager& get(void) noexcept
            {
                static LuaManager manager;

                return manager;
            }
    };  // class LuaManagerSingletone

}  // namespace RType
#endif  // SRC_GAME_LUAMANAGER_HPP_
