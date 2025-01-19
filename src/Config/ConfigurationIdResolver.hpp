//
#pragma once

#include "src/Config/EntityConfigResolver.hpp"
#include <exception>
#include <rengine/Rengine.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <string>

namespace RType {
namespace Config {
class ConfigurationIdResolverException : public std::exception {
public:
    explicit ConfigurationIdResolverException(const std::string& msg)
    {
        std::string head = "RType::Config::ConfigurationIdResolver: ";
        this->_concat = head + msg;
    };
    const char *what() const noexcept
    {
        return this->_concat.c_str();
    };
private:
    std::string _concat;
};
/**
* @brief A class holding a map of uint16_t and the association json path
*/
class ConfigurationIdResolver {
    public:
        explicit ConfigurationIdResolver(const std::string& jsonPath)
            :_jsonPath(jsonPath)
        {
            std::ifstream f;
            try {
                f.open(this->_jsonPath);
            } catch (std::exception &e) {
                std::string msg = e.what();
                throw ConfigurationIdResolverException("Error on " + jsonPath + ":" + msg);
            }
            nlohmann::json j;
            // Parsing file into j
            try {
                j = nlohmann::json::parse(f);
            } catch (std::exception &e) {
                std::string msg = e.what();
                throw ConfigurationIdResolverException("Error on " + jsonPath + ":" + msg);
            }
            uint16_t index = 0;
            for (auto& it : j.items()) {
                std::string sId = it.key();
                std::string path = it.value();
                try {
                    this->_map[path] = std::stoi(sId);
                } catch (std::exception& e) {
                    std::string msg = e.what();
                    std::cout << "Got error on element " << sId  << ": " << msg << std::endl;
                }
                index += 1;
            }
            f.close();
        }
        std::unordered_map<std::string, uint16_t>::const_iterator begin(void) const noexcept
        {
            return this->_map.begin();
        }
        std::unordered_map<std::string, uint16_t>::const_iterator end(void) const noexcept
        {
            return this->_map.end();
        }
        /**
         * @brief Get the path linked to id or "" if not found.
         */
        const std::string get(uint16_t id) const noexcept
        {
            for (auto it : this->_map) {
                if (it.second == id) {
                    return it.first;
                }
            }
            return "";
        }
        /**
         * @brief Get the id linked to the path or -1 if not found.
         */
        uint16_t get(const std::string& jsonPath) const noexcept
        {
            for (auto it : this->_map) {
                if (it.first == jsonPath) {
                    return it.second;
                }
            }
            return -1;
        }

    private:
        std::string _jsonPath = "";
        std::unordered_map<std::string, uint16_t> _map;
};

class LevelConfigurationIdResolverSingletone {
    public:
        LevelConfigurationIdResolverSingletone(void) = default;
        ~LevelConfigurationIdResolverSingletone(void) = default;
        static const ConfigurationIdResolver& get(void) noexcept
        {
            static ConfigurationIdResolver obj("assets/levels/id.json");
            return obj;
        }
};

class EntityConfigurationIdResolverSingletone {
    public:
        EntityConfigurationIdResolverSingletone(void) = default;
        ~EntityConfigurationIdResolverSingletone(void) = default;
        static const ConfigurationIdResolver& get(void) noexcept
        {
            static ConfigurationIdResolver obj("assets/entities/id.json");
            return obj;
        }
};
}  // namespace Config
}  // namespace RType
