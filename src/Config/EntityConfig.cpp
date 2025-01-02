//
//
#include <exception>
#include <nlohmann/json.hpp>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>

#include "EntityConfig.hpp"

namespace RType {

    namespace Config {


        EntityConfig::EntityConfig(const std::string& jsonPath)
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
                j = j["entity"];  // Get entity general field
                this->parseSprite(j["sprite"]);
                this->parseHitbox(j["hitbox"]);
                this->parseStats(j["stats"]);
                this->parseCharacteristics(j["characteristics"]);
                this->parseAttacks(j["attacks"]);
            }
            // Error on parsing
            catch (std::exception &e) {
                throw EntityConfigExceptionInvalidJsonFile(jsonPath, e.what());
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
        const EntityConfigSprite& EntityConfig::getSprite(void) const noexcept
        {
            return this->_sprite;
        }
        const std::optional<AttackConfig>& EntityConfig::getAttack(uint8_t id) const noexcept
        {
            if (id == 0 || id > 3) {
                id = 3;
            }
            return this->_attacks[id - 1];
        }

        void EntityConfig::parseSprite(nlohmann::json& spriteField)
        {
            // TextureScale
            try {
                float scaleX = spriteField["scale"]["x"];
                float scaleY = spriteField["scale"]["y"];

                this->_sprite.textureScale = {scaleX, scaleY};
            } catch (std::exception &e) {
                this->_sprite.textureScale = {1, 1};
            }
            // OriginOffset
            try {
                this->_sprite.originOffset = {spriteField["origin"]["x"], spriteField["origin"]["y"]};
            } catch (std::exception &e) {
                this->_sprite.originOffset = {0, 0};
            }
            this->_sprite.texturePath = spriteField["texturePath"];
            // Animation
            try {
                nlohmann::json animationField = spriteField["animations"];

                this->_sprite.animation.frameRectXY = {animationField["frameRect"]["x"], animationField["frameRect"]["y"]};
                this->_sprite.animation.frameRectWidthHeight = {animationField["frameRect"]["width"], animationField["frameRect"]["height"]};
                this->_sprite.animation.frameCount = animationField["frameCount"];
                this->_sprite.animation.frameDuration = animationField["frameDuration"];
                this->_sprite.animation.frameDisplacement = {animationField["frameDisplacement"]["x"], animationField["frameDisplacement"]["y"]};
            } catch (std::exception &e) {
                std::string msg = e.what();

                throw std::runtime_error("An error occured when parsing 'animations':" + msg);
            }
        }
        void EntityConfig::parseHitbox(nlohmann::json& hitboxField)
        {
            this->_hitbox.size = {hitboxField["size"]["x"], hitboxField["size"]["y"]};
            try {
                this->_hitbox.offsetFromSpriteOrigin = {hitboxField["offset"]["x"], hitboxField["offset"]["y"]};
            } catch (std::exception &e) {
                this->_hitbox.offsetFromSpriteOrigin = {0, 0};
            }
        }
        void EntityConfig::parseStats(nlohmann::json& statsField)
        {
            try {
                this->_stats.hp = statsField["hp"];
                this->_stats.attack = statsField["attack"];
                this->_stats.defense = statsField["defense"];
                this->_stats.minSpeed = statsField["minSpeed"];
                this->_stats.acceleration = statsField["acceleration"];
                this->_stats.maxSpeed = statsField["maxSpeed"];
                this->_stats.enemyColisionDamage = statsField["enemyColisionDamage"];
            } catch (std::exception& e) {
                std::string msg = e.what();

                throw std::runtime_error("An error occured when parsing 'stats':" + msg);
            }
        }
        void EntityConfig::parseCharacteristics(nlohmann::json& charaField)
        {
            // enemyDamageOnColision
            try {
                this->_characteristics.enemyDamageOnColision = charaField["enemyDamageOnColision"];
            } catch (std::exception& e) {
                ;
            }
            // damagedOnColision
            try {
                this->_characteristics.damagedOnColision = charaField["damagedOnColision"];
            } catch (std::exception& e) {
                ;
            }
            // persistsOffScreen
            try {
                this->_characteristics.persistsOffScreen = charaField["persistsOffScreen"];
            } catch (std::exception& e) {
                ;
            }
        }
        void EntityConfig::parseAttacks(nlohmann::json& attacksField)
        {
            // Shoot1
            try {
                this->_attacks[0] = AttackConfig(attacksField["shoot1"]);
            } catch (std::exception &e) {
                ;
            }
            // Shoot2
            try {
                this->_attacks[1] = AttackConfig(attacksField["shoot2"]);
            } catch (std::exception &e) {
                ;
            }
            // Shoot1
            try {
                this->_attacks[2] = AttackConfig(attacksField["shoot3"]);
            } catch (std::exception &e) {
                ;
            }
        }
    }
}
