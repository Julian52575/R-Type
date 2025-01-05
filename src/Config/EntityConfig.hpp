//
//
#ifndef _SRC_CONFIG_ENTITYCONFIG_HPP_
#define _SRC_CONFIG_ENTITYCONFIG_HPP_
#include <cstdint>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <utility>
#include <optional>

#include "./AttackConfig.hpp"
#include "./ImageConfig.hpp"

namespace RType {

    namespace Config {

        class EntityConfigExceptionFileError : public std::exception {
        public:
            EntityConfigExceptionFileError(const std::string& fileName, const char *jsonMsg)
            {
                std::string msg = "RType::Config::EntityConfig An error happened when trying to read from '" + fileName + "'.";

                this->_concat = msg + jsonMsg;
            };
            const char *what() const noexcept
            {
                return this->_concat.c_str();
            };

        private:
            std::string _concat;
        };
        class EntityConfigExceptionInvalidJsonFile : public std::exception {
        public:
            EntityConfigExceptionInvalidJsonFile(const std::string& fileName, const char *jsonMsg)
            {
                std::string msg = "RType::Config::EntityConfig Trying to construct from an invalid json file '" + fileName + "'.\nError: '";

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
        * @class EntityConfigStats
        * @brief A structure of the fields specified for the entity's stats.
        */
#warning Separate X and Y speed
        struct EntityConfigStats {
            uint16_t hp;
            uint16_t attack;
            uint16_t defense;
            uint16_t maxSpeed;
            uint16_t minSpeed;
            float acceleration;
            uint16_t enemyColisionDamage;
            uint16_t ownCollisionDamage;
        };
        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class EntityConfigCharacteristics
        * @brief A structure of the fields specified for the entity's characteristics.
        */
        struct EntityConfigCharacteristics {
            bool enemyDamageOnColision = false;
            bool damagedOnColision = false;
            bool persistsOffScreen = false;
        };
        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class EntityConfigHitbox
        * @brief A structure of the fields specified for the entity's hitbox.
        */
        struct EntityConfigHitbox {
            std::pair<uint16_t, uint16_t> size;
            std::pair<uint16_t, uint16_t> offsetFromSpriteOrigin;
        };
        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class EntityConfig
        * @brief This class encapsulate the entity json.
        * Is used to convert a jsonPath to a class of c++ variable.
        */
        class EntityConfig {
            public:
                /**
                * @fn EntityConfig
                * @brief Do not call this default constructor, it will create an empty EntityConfig.
                */
                EntityConfig(void) = default;
                /**
                * @fn EntityConfig
                * @param jsonPath The path to a json entity configuration.
                * The entity configuration must start with the 'entity' field.
                */
                EntityConfig(const std::string& jsonPath);
                ~EntityConfig(void) = default;
                /**
                * @fn getStats
                * @return EntityConfigStats A structure of the entity's stats.
                * @brief Returns the stats of the entity.
                */
                const EntityConfigStats& getStats(void) const noexcept;
                /**
                * @fn getCharacteristics
                * @return EntityConfigCharacteristics A structure of the entity's characteristics.
                * @brief Returns the characteristics of the entity.
                */
                const EntityConfigCharacteristics& getCharacteristics(void) const noexcept;
                /**
                * @fn getHitbox
                * @return EntityConfigHitbox A structure of the entity's hitbox.
                * @brief Returns the hitbox of the entity.
                */
                const EntityConfigHitbox& getHitbox(void) const noexcept;
                /**
                * @fn getSprite
                * @return EntityConfigSprite A structure of the entity's sprite.
                * @brief Returns the data of the entity's sprite.
                */
                const ImageConfig& getSprite(void) const noexcept;
                /**
                * @fn getAttack
                * @return std::optional<AttackConfig> An optional AttackConfig.
                * @param id The shoot id, following this table:
                * 0 = shoot1
                * 1 = shoot1
                * 2 = shoot2
                * 3 = shoot3
                * 4+ = shoot3
                * @brief Returns the data for the entity's attack.
                */
                const std::optional<AttackConfig>& getAttack(uint8_t id) const noexcept;

            private:
                ImageConfig _sprite;
                EntityConfigStats _stats;
                EntityConfigCharacteristics _characteristics;
                EntityConfigHitbox _hitbox;
                std::optional<AttackConfig> _attacks[3];

            private:
                void parseHitbox(nlohmann::json& hitboxField);
                void parseStats(nlohmann::json& statsField);
                void parseCharacteristics(nlohmann::json& charaField);
                void parseAttacks(nlohmann::json& attacksField);
        };  // class EntityConfig

    }  // namespace Config

}  // namespace RType
#endif
