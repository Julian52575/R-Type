//
//
#include <exception>
#include <nlohmann/json.hpp>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>

#include "./AttackConfig.hpp"
#include "./AttackConfigResolver.hpp"
#include "./ImageConfig.hpp"
#include "./ImageConfigResolver.hpp"
#include "EntityConfig.hpp"

namespace RType {

    namespace Config {


        EntityConfig::EntityConfig(const std::string& jsonPath)
        {
            this->parseJson(jsonPath);
        }
        void EntityConfig::parseJson(const std::string& jsonPath)
        {
            std::ifstream f;

            try {
                f.open(jsonPath);
            } catch (std::exception &e) {
                throw EntityConfigExceptionFileError(jsonPath, e.what());
            }
            nlohmann::json j;

            try {
                j = nlohmann::json::parse(f);
            } catch (std::exception &e) {
                throw EntityConfigExceptionInvalidJsonFile(jsonPath, e.what());
            }

            if (j.contains("entity") == false) {
                throw EntityConfigExceptionInvalidJsonFile(jsonPath, "No 'entity' field found.");
            }
            j = j["entity"];
            // sprite
            if (j.contains("sprite") == true) {
                ImageConfigResolverSingletone imageSingletone;
                ImageConfigResolver imageResolver = imageSingletone.get();

                this->_sprite = imageResolver.get(j["sprite"]);
            }
            // hitbox
            if (j.contains("hitbox") == true) {
                this->parseHitbox(j["hitbox"]);
            }
            // stats
            if (j.contains("stats") == true) {
                this->parseStats(j["stats"]);
            }
            // characteristics
            if (j.contains("characteristics") == true) {
                this->parseCharacteristics(j["characteristics"]);
            }
            // attacks
            if (j.contains("attacks") == true) {
                this->parseAttacks(j["attacks"]);
            }
        }

        const EntityConfigStats& EntityConfig::getStats(void) const noexcept
        {
            return this->_stats;
        }
        const EntityConfigCharacteristics& EntityConfig::getCharacteristics(void) const noexcept
        {
            return this->_characteristics;
        }
        const EntityConfigHitbox& EntityConfig::getHitbox(void) const noexcept
        {
            return this->_hitbox;
        }
        const ImageConfig& EntityConfig::getSprite(void) const noexcept
        {
            return this->_sprite;
        }
        const std::optional<AttackConfig>& EntityConfig::getAttack(uint8_t id) const noexcept
        {
            if (id == 0 || id > 3) {
                id = 3;
            }
            if (id == 0) {
                id = 1;
            }
            return this->_attacks[id - 1];
        }

        void EntityConfig::parseHitbox(nlohmann::json& hitboxField)
        {
            // size
            if (hitboxField.contains("size") == true) {
                nlohmann::json& sizeField = hitboxField["size"];

                // x && y
                if (sizeField.contains("x") == true) {
                    this->_hitbox.size.x = sizeField["x"];
                }
                if (sizeField.contains("y") == true) {
                    this->_hitbox.size.y = sizeField["y"];
                }
            }
            if (hitboxField.contains("offset") == true) {
                nlohmann::json& offsetField = hitboxField["offset"];

                // x && y
                if (offsetField.contains("x") == true) {
                    this->_hitbox.offsetFromSpriteOrigin.x = offsetField["x"];
                }
                if (offsetField.contains("y") == true) {
                    this->_hitbox.offsetFromSpriteOrigin.y = offsetField["y"];
                }
            }
        }
        void EntityConfig::parseStats(nlohmann::json& statsField)
        {
            // hp
            if (statsField.contains("hp") == true) {
                this->_stats.hp = statsField["hp"];
            }
            // attack
            if (statsField.contains("attack") == true) {
                this->_stats.attack = statsField["attack"];
            }
            // defense
            if (statsField.contains("defense") == true) {
                this->_stats.defense = statsField["defense"];
            }
            // speedY
            if (statsField.contains("speedY") == true) {
                this->_stats.speedY = statsField["speedY"];
            }
            // acceleration
            if (statsField.contains("acceleration") == true) {
                this->_stats.acceleration = statsField["acceleration"];
            }
            // speedX
            if (statsField.contains("speedX") == true) {
                this->_stats.speedX = statsField["speedX"];
            }
            // enemyColisionDamage
            if (statsField.contains("enemyColisionDamage") == true) {
                this->_stats.enemyColisionDamage = statsField["enemyColisionDamage"];
            } else {
                this->_stats.enemyColisionDamage = this->_stats.attack;
            }
        }
        void EntityConfig::parseCharacteristics(nlohmann::json& charaField)
        {
            // enemyDamageOnColision
            if (charaField.contains("enemyDamageOnColision") == true) {
                this->_characteristics.enemyDamageOnColision = charaField["enemyDamageOnColision"];
            }
            // damagedOnColision
            if (charaField.contains("damagedOnColision") == true) {
                this->_characteristics.damagedOnColision = charaField["damagedOnColision"];
            }
            // persistsOffScreen
            if (charaField.contains("persistsOffScreen") == true) {
                this->_characteristics.persistsOffScreen = charaField["persistsOffScreen"];
            }
        }
        void EntityConfig::parseAttacks(nlohmann::json& attacksField)
        {
            AttackConfigResolverSingletone singletone;
            AttackConfigResolver& resolver = singletone.get();

            // Shoot1
            if (attacksField.contains("shoot1") == true) {
                this->_attacks[0] = resolver.get(attacksField["shoot1"]);
            }
            // Shoot2
            if (attacksField.contains("shoot2") == true) {
                this->_attacks[1] = resolver.get(attacksField["shoot2"]);
            }
            // Shoot3
            if (attacksField.contains("shoot3") == true) {
                this->_attacks[2] = resolver.get(attacksField["shoot3"]);
            }
        }
    }  // namespace Config
}  // namespace RType
