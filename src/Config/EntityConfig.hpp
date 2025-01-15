//
#ifndef SRC_CONFIG_ENTITYCONFIG_HPP_
#define SRC_CONFIG_ENTITYCONFIG_HPP_
#include <cstdint>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <rengine/src/Graphics/Vector.hpp>
#include <stdexcept>
#include <utility>
#include <optional>
#include <string>

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
        struct EntityConfigStats {
            uint16_t hp = 100;
            uint16_t attack = 10;
            uint16_t defense = 10;
            uint16_t speedX = 10;
            uint16_t speedY = 10;
            float acceleration = 0.5;
            uint16_t enemyColisionDamage = 10;
            uint16_t ownCollisionDamage = 10;
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
            Rengine::Graphics::vector2D<uint16_t> size = {25, 25};
            Rengine::Graphics::vector2D<float> offsetFromSpriteOrigin = {0, 0};
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
                * @brief Create an empty EntityConfig.
                */
                EntityConfig(void) = default;
                /**
                * @fn EntityConfig
                * @param jsonPath The path to a json entity configuration.
                * The entity configuration must start with the 'entity' field.
                */
                explicit EntityConfig(const std::string& jsonPath);
                /**
                * @fn parseJson
                * @param jsonPath The path to a json entity configuration.
                * The entity configuration must start with the 'entity' field.
                */
                void parseJson(const std::string& jsonPath);
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
#endif  // SRC_CONFIG_ENTITYCONFIG_HPP_
