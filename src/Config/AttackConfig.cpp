// Objective: Json to AttackConfig
//
//
#include <exception>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

#include "./AttackConfig.hpp"
#include "./MissileConfig.hpp"
#include "./BuffConfig.hpp"
#include "Enum.hpp"

namespace RType {

    namespace Config {

        void AttackConfig::parseGeneral(nlohmann::json &attackField)
        {
            // Parse cooldown
            this->_cooldown = attackField["cooldown"];
            // Parse type
            std::string type = attackField["type"];
            std::vector<std::pair<std::string, RType::Config::AttackType>> bind = {
                {"buff", RType::Config::AttackType::AttackTypeBuffs},
                {"missiles", RType::Config::AttackType::AttackTypeMissiles}
            };
            auto it = bind.begin();
            while (it != bind.end()) {
                if (it->first == type) {
                    this->_type = it->second;
                    return;
                }
                it++;
            }
            if (it == bind.end()) {
                throw std::runtime_error("Unknow attack.type.");
            }
        }
        void AttackConfig::parseBuffs(nlohmann::json &buffsField)
        {
            if (buffsField.size() == 0) {
                throw std::runtime_error("Empty 'buffs' field.");
            }
            auto vector = std::vector<BuffConfig>(buffsField.size());
            std::vector<int>::size_type count = 0;

            // Create a new RType::Config::BuffData base on the current field of the 'buffs' field.
            for (auto it : buffsField) {
                vector[count] = std::move(BuffConfig(it));
                count += 1;
            }
            this->_buffsVector = std::move(vector);
        }
        void AttackConfig::parseMissiles(nlohmann::json &missilesField)
        {
            if (missilesField.size() == 0) {
                throw std::runtime_error("Empty 'missiles' field.");
            }
            auto vector = std::vector<MissileConfig>(missilesField.size());
            std::vector<int>::size_type count = 0;

            for (auto it : missilesField) {
                vector[count] = std::move(MissileConfig(it));
                count += 1;
            }
            this->_missilesVector = std::move(vector);
        }

        AttackConfig::AttackConfig(const std::string& jsonPath)
        {
            std::ifstream f;

            try {
                f.open(jsonPath);
            } catch (std::exception &e) {
                throw AttackConfigExceptionFileError(jsonPath, e.what());
            }
            nlohmann::json j;

            try {
                j = nlohmann::json::parse(f);

                // Parse general fields
                this->parseGeneral(j["attack"]);
                j = j["attack"];
                // Buff type
                if (this->_type == AttackType::AttackTypeBuffs) {
                    this->parseBuffs(j["buffs"]);
                }
                // Shoot type
                else if (this->_type == AttackType::AttackTypeMissiles) {
                    this->parseMissiles(j["missiles"]);
                }
                // Unknow type
                else {
                    throw std::runtime_error("Invalid type.");
                }
            } catch (std::exception &e) {
                throw AttackConfigExceptionInvalidJsonFile(jsonPath, e.what());
            }
        }
        AttackType AttackConfig::getType(void) const noexcept
        {
            return this->_type;
        }
        const std::vector<BuffConfig>& AttackConfig::getBuffs(void) const
        {
            if (this->_type != AttackType::AttackTypeBuffs || this->_buffsVector.has_value() == false) {
                throw AttackConfigExceptionWrongTypeAccess();
            }
            return this->_buffsVector.value();
        }
        const std::vector<MissileConfig>& AttackConfig::getMissiles(void) const
        {
            if (this->_type != AttackType::AttackTypeMissiles || this->_missilesVector.has_value() == false) {
                throw AttackConfigExceptionWrongTypeAccess();
            }
            return this->_missilesVector.value();
        }
    }  // namespace Config

}  // namespace RType
