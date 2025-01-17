#include <bits/types/struct_tm.h>
#include <lua.hpp>  // <lua5.4/lua.hpp>  // changed for CMake
#include <utility>
#include <iostream>
#include <vector>
#include <string>

#include "LuaManager.hpp"
namespace RType {

    LuaType luaConstantToLuaType(int lua_type_)
    {
        switch (lua_type_) {
            case LUA_TNIL:
                return LuaType::LuaTypeNA;

            case LUA_TNUMBER:
                return LuaTypeInt;

            case LUA_TBOOLEAN:
                return LuaTypeBoolean;

            case LUA_TSTRING:
                return LuaTypeString;

            case LUA_TTABLE:
                return LuaTypeTable;

            case LUA_TFUNCTION:
                return LuaTypeFunction;

            case LUA_TUSERDATA:
                return LuaTypeUserData;

            case LUA_TTHREAD:
                return LuaTypeThread;

            case LUA_TLIGHTUSERDATA:
                return LuaTypeLightUserData;

            default:
                return LuaTypeNA;
        }
    }

    LuaManager::LuaManager(void)
    {
    }

    LuaManager::~LuaManager(void)
    {
        for (auto& [name, state] : this->states) {
            for (size_t i = 0; i < state.size(); i++) {
                if (state[i] != nullptr) {
                    lua_close(state[i]);
                }
            }
        }
    }

    int LuaManager::loadLuaScript(const std::string& filename)
    {
        lua_State* L = luaL_newstate();

        if (!L) {
            throw LuaManagerException("Lua error: cannot create Lua state");
        }
        luaL_openlibs(L);
        if (luaL_dofile(L, filename.c_str()) != LUA_OK) {
            lua_pop(L, 1);
            throw LuaManagerException("Lua error: Cannot load file: " + filename);
        }

        if (this->states.find(filename) == this->states.end()) {
            this->states[filename] = {L};
            this->freeIds[filename] = std::queue<int>();
            return 0;
        }

        if (this->freeIds.find(filename) != this->freeIds.end() && !this->freeIds[filename].empty()) {
            int id = this->freeIds[filename].front();
            this->freeIds[filename].pop();
            this->states[filename][id] = L;
            return id;
        }

        this->states[filename].push_back(L);
        return this->states[filename].size() - 1;
    }

    void LuaManager::removeScriptAtIndex(const std::string& filename, int id){
        if (this->states.find(filename) == this->states.end()) {
            throw LuaManagerException("Lua error: "+filename+" not found");
        }
        if (this->states[filename].size() <= id) {
            throw LuaManagerException("Lua error: "+filename+" id "+std::to_string(id)+" not found");
        }
        
        if (this->states[filename][id] != nullptr) {
            lua_close(this->states[filename][id]);
            this->states[filename][id] = nullptr;
            this->freeIds[filename].push(id);
        }
    }

    void LuaManager::push(lua_State *)
    {
    }

    void LuaManager::push(lua_State *L, const std::string& value)
    {
        lua_pushstring(L, value.c_str());
    }

    void LuaManager::push(lua_State *L, const char* value)
    {
        lua_pushstring(L, value);
    }

    void LuaManager::push(lua_State *L, int value)
    {
        lua_pushinteger(L, value);
    }

    void LuaManager::push(lua_State *L, float value)
    {
        lua_pushnumber(L, value);
    }

    void LuaManager::push(lua_State *L, double value)
    {
        lua_pushnumber(L, value);
    }

    void LuaManager::push(lua_State *L, bool value)
    {
        lua_pushboolean(L, value);
    }

    void LuaManager::push(lua_State *L, std::nullptr_t)
    {
        lua_pushnil(L);
    }

    void LuaManager::push(lua_State *L, const std::any& value)
    {
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
            throw LuaManagerException(std::string("Lua error: unsupported type: ") + std::string(typeid(value).name()) + " for std::any");
        }
    }

    std::unordered_map<std::string, std::any> LuaManager::processLuaTable(lua_State* L, int index)
    {
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
                    throw LuaManagerException("Lua error: unknown type");
            }
            lua_pop(L, 1);
        }
        return result;
    }

    std::vector<LuaReturn> LuaManager::getLuaResult(lua_State* L)
    {
        std::vector<LuaReturn> result;
        int nbr_results = lua_gettop(L);

        if (nbr_results == 0) {
            return result;
        }
        for (int i = 1; i <= nbr_results; i++) {
            LuaReturn r;

            r.type = LuaTypeNA;
            r.data = {0};
            switch (lua_type(L, i)) {
                case LUA_TNUMBER:
                    r.type = LuaTypeInt;
                    r.data.integer = (int64_t) lua_tonumber(L, i);
                    break;

                case LUA_TBOOLEAN:
                    r.type = LuaTypeBoolean;
                    r.data.boolean = bool(lua_toboolean(L, i));
                    break;

                case LUA_TSTRING:
                    r.type = LuaTypeString;
                    r.data.str = lua_tostring(L, i);
                    break;

                case LUA_TNIL:
                    r.type = LuaTypeNA;
                    break;

                case LUA_TTABLE:
                    //processLuaTable(L, i);
                    #warning Lua table ignored
                    continue;

                default:
                    throw LuaManagerException("Lua error: unknown type");
            }  // switch
            result.push_back(r);
        }  // for
        lua_pop(L, nbr_results);
        return result;
    }

}  // namespace RType
