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

#include "./Enum.hpp"
#include "./AttackConfig.hpp"

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

        struct EntityConfigCharacteristics {
            bool enemyDamageOnColision = false;
            bool damagedOnColision = false;
            bool persistsOffScreen = false;
        };

        struct EntityConfigHitbox {
            std::pair<uint16_t, uint16_t> size;
            std::pair<uint16_t, uint16_t> offsetFromSpriteOrigin;
        };

        struct EntityConfigSprite {
            std::string texturePath;
            std::pair<uint16_t, uint16_t> originOffset;
            std::pair<float, float> textureScale;

            struct EntityConfigSpriteAnimation {
                std::pair<uint16_t, uint16_t> frameRectXY;
                std::pair<uint16_t, uint16_t> frameRectWidthHeight;
                uint16_t frameCount;
                float frameDuration;
                std::pair<uint16_t, uint16_t> frameDisplacement;
            };
            EntityConfigSpriteAnimation animation;
        };

        class EntityConfig {
            public:
                EntityConfig(void) = default;
                EntityConfig(const std::string& jsonPath);
                ~EntityConfig(void) = default;
                const EntityConfigStats& getStats(void) const noexcept;
                const EntityConfigCharacteristics& getCharacteristics(void) const noexcept;
                const EntityConfigHitbox& getHitbox(void) const noexcept;
                const EntityConfigSprite& getSprite(void) const noexcept;
                const std::optional<AttackConfig>& getAttack(uint8_t id) const noexcept;

            private:
                EntityConfigStats _stats;
                EntityConfigCharacteristics _characteristics;
                EntityConfigHitbox _hitbox;
                EntityConfigSprite _sprite;
                std::optional<AttackConfig> _attacks[3];

            private:
                void parseSprite(nlohmann::json& spriteField);
                void parseHitbox(nlohmann::json& hitboxField);
                void parseStats(nlohmann::json& statsField);
                void parseCharacteristics(nlohmann::json& charaField);
                void parseAttacks(nlohmann::json& attacksField);
        };  // class EntityConfig

    }  // namespace Config

}  // namespace RType
#endif
