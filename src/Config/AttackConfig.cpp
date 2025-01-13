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
#include "./AttackBuffTypeEnum.hpp"

namespace RType {

    namespace Config {

        void AttackConfig::parseGeneral(nlohmann::json &attackField)
        {
            if (attackField.contains("type") == false) {
                throw std::runtime_error("No 'type' field.");
            }
            // Parse cooldown
            if (attackField.contains("cooldown") == true) {
                this->_cooldown = attackField["cooldown"];
                if (this->_cooldown == 0.0f) {
                    this->_cooldown = 0.1f;
                }
            }
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
            auto vector = std::vector<RType::Config::BuffConfig>(buffsField.size());
            std::vector<int>::size_type count = 0;

            // Create a new RType::Config::BuffData base on the current field of the 'buffs' field.
            for (auto it : buffsField) {
                vector[count] = std::move(RType::Config::BuffConfig(it));
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

            // Parsing file into j
            try {
                j = nlohmann::json::parse(f);
            } catch (std::exception &e) {
                throw AttackConfigExceptionInvalidJsonFile(jsonPath, e.what());
            }
            // No attack field
            if (j.contains("attack") == false) {
                throw AttackConfigExceptionInvalidJsonFile(jsonPath, "No 'attack' field.");
            }
            j = j["attack"];
            // Parse general fields
            try {
                this->parseGeneral(j);
            } catch (std::exception& e) {
                throw AttackConfigExceptionInvalidJsonFile(jsonPath, e.what());
            }
            // Buff type
            if (this->_type == AttackType::AttackTypeBuffs) {
                // No buffs field
                if (j.contains("buffs") == false) {
                    throw AttackConfigExceptionInvalidJsonFile(jsonPath, "No 'buffs' field on 'type' = 'buff'.");
                }
                try {
                    this->parseBuffs(j["buffs"]);
                } catch (std::exception& e) {
                    throw AttackConfigExceptionInvalidJsonFile(jsonPath, e.what());
                }
            } else if (this->_type == AttackType::AttackTypeMissiles) {  // Shoot type
                // No missiles field
                if (j.contains("missiles") == false) {
                    throw AttackConfigExceptionInvalidJsonFile(jsonPath, "No 'missiles' field on 'type' = 'buff'.");
                }
                try {
                    this->parseMissiles(j["missiles"]);
                } catch (std::exception& e) {
                    throw AttackConfigExceptionInvalidJsonFile(jsonPath, e.what());
                }
            } else {  // Unknow type
                throw AttackConfigExceptionInvalidJsonFile(jsonPath, "Unknow 'type'. Should be 'missiles' or 'buffs'.");
            }
        }
        AttackType AttackConfig::getType(void) const noexcept
        {
            return this->_type;
        }
        const std::vector<RType::Config::BuffConfig>& AttackConfig::getBuffs(void) const
        {
            if (this->_type != AttackType::AttackTypeBuffs || this->_buffsVector.has_value() == false) {
                throw AttackConfigExceptionWrongTypeAccess();
            }
            return this->_buffsVector.value();
        }
        const std::vector<RType::Config::MissileConfig>& AttackConfig::getMissiles(void) const
        {
            if (this->_type != AttackType::AttackTypeMissiles || this->_missilesVector.has_value() == false) {
                throw AttackConfigExceptionWrongTypeAccess();
            }
            return this->_missilesVector.value();
        }
    }  // namespace Config

}  // namespace RType
