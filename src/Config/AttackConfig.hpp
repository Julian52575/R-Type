// Objective: Json to AttackConfig
//
//
#include <cstddef>
#include <string>
#include <utility>
#include <optional>
#include <vector>
#include <nlohmann/json.hpp>

#include "./MissileConfig.hpp"
#include "./BuffConfig.hpp"

namespace RType {

    namespace Config {

        class AttackConfigExceptionAccessBuffOnShootType : public std::exception {
        public:
            const char *what() const noexcept {
                return "RType::Config::AttackConfig Trying to access buffs on shoot attack type.";
            };
        };
        class AttackConfigExceptionAccessShootOnBuffType : public std::exception {
        public:
            const char *what() const noexcept {
                return "RType::Config::AttackConfig Trying to access shoots on buff attack type.";
            };
        };
        class AttackConfigExceptionFileError : public std::exception {
        public:
            AttackConfigExceptionFileError(const std::string& fileName, const char *jsonMsg)
            {
                std::string msg = "RType::Config::AttackConfig An error happened when trying to read from '" + fileName + "'.";

                this->_concat = msg + jsonMsg;
            };
            const char *what() const noexcept
            {
                return this->_concat.c_str();
            };

        private:
            std::string _concat;
        };
        class AttackConfigExceptionInvalidJsonFile : public std::exception {
        public:
            AttackConfigExceptionInvalidJsonFile(const std::string& fileName, const char *jsonMsg)
            {
                std::string msg = "RType::Config::AttackConfig Trying to construct from an invalid json file '" + fileName + "'.\nError: '";

                this->_concat = msg + jsonMsg + "'.";
            };
            const char *what() const noexcept
            {
                return this->_concat.c_str();
            };

        private:
            std::string _concat;
        };
        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class AttackConfig
        * @brief This class encapsulate the attack json.
        * Is used to convert a jsonPath to a class of c++ variable.
        */
        class AttackConfig {
            public:
                /**
                * @fn AttackConfig
                * @exception AttackConfigExceptionFileError An error happened when reading jsonPath.
                * @exception AttackConfigExceptionInvalidJsonFile jsonPath has an error.
                * @brief Create a c++ class from the path of an attack json.
                */
                AttackConfig(const std::string& jsonPath);
                ~AttackConfig(void) = default;
                /**
                * @fn getType
                * @return RType::Config::AttackType The type of the attack. See RType::Config::AttackType.
                * @brief Return the type of the attack.
                */
                AttackType getType(void) const noexcept;
                /**
                * @fn getBuffs
                * @exception AttackConfigExceptionAccessBuffOnShootType This function is called on a Shoot type attack.
                * @brief Get the vector of RType::Config::BuffData
                */
                const std::vector<BuffConfig>& getBuffs(void) const;
                /**
                * @fn getMissiles
                * @exception AttackConfigExceptionAccessShootOnBuffType This function is called on a Buff type attack.
                * @brief Get the vector of RType::Config::MissileData
                */
                const std::vector<MissileConfig>& getMissiles(void) const;
                /**
                * @fn getCooldown
                * @return RType::Config::AttackType The type of the attack. See RType::Config::AttackType.
                * @brief Return the cooldown of the attack.
                */
                double getCooldown(void) const noexcept
                {
                    return this->_cooldown;
                }

            private:
                /**
                * @fn parseGeneral
                * @exception ??? a
                * @brief Update this class by parsing the general fields.
                */
                void parseGeneral(nlohmann::json &attackField);
                /**
                * @fn parseBuffs
                * @exception ??? a
                * @brief Update this class by parsing the buff specifics fields.
                */
                void parseBuffs(nlohmann::json &buffsField);
                /**
                * @fn parseMissiles
                * @exception ??? a
                * @brief Update this class by parsing the missiles specifics fields.
                */
                void parseMissiles(nlohmann::json &missilesField);

            private:
                AttackType _type = AttackType::AttackTypeNA;
                double _cooldown = 0.0;
                // Buff + cooldown
                std::optional<std::vector<BuffConfig>> _buffsVector;
                // Missile json -> <offset, velocity>
                std::optional<std::vector<MissileConfig>> _missilesVector;
        };
    }
}
