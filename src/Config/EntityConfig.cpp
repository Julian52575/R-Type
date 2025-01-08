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
            std::ifstream f;

            try {
                f.open(jsonPath);
            } catch (std::exception &e) {
                throw EntityConfigExceptionFileError(jsonPath, e.what());
            }
            nlohmann::json j;

            try {
                j = nlohmann::json::parse(f);
                if (j.contains("entity") == false) {
                    throw EntityConfigExceptionInvalidJsonFile(jsonPath, "No 'entity' field found.");
                }
                j = j["entity"];
                // sprite
                if (j.contains("sprite")) {
                    ImageConfigResolverSingletone imageSingletone;
                    ImageConfigResolver imageResolver = imageSingletone.get();

                    this->_sprite = imageResolver.get(j["sprite"]);
                }
                // hitbox
                if (j.contains("hitbox")) {
                    this->parseHitbox(j["hitbox"]);
                }
                // stats
                if (j.contains("stats")) {
                    this->parseStats(j["stats"]);
                }
                // characteristics
                if (j.contains("characteristics")) {
                    this->parseCharacteristics(j["characteristics"]);
                }
                // attacks
                if (j.contains("attacks")) {
                    this->parseAttacks(j["attacks"]);
                }
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
            } catch (std::exception& e) {;}
            // damagedOnColision
            try {
                this->_characteristics.damagedOnColision = charaField["damagedOnColision"];
            } catch (std::exception& e) {;}
            // persistsOffScreen
            try {
                this->_characteristics.persistsOffScreen = charaField["persistsOffScreen"];
            } catch (std::exception& e) {;}
        }
        void EntityConfig::parseAttacks(nlohmann::json& attacksField)
        {
            AttackConfigResolverSingletone singletone;
            AttackConfigResolver& resolver = singletone.get();

            // Shoot1
            try {
                this->_attacks[0] = resolver.get(attacksField["shoot1"]);
            } catch (std::exception &e) {;}
            // Shoot2
            try {
                this->_attacks[1] = resolver.get(attacksField["shoot2"]);
            } catch (std::exception &e) {;}
            // Shoot1
            try {
                this->_attacks[2] = resolver.get(attacksField["shoot3"]);
            } catch (std::exception &e) {;}
        }
    }  // namespace Config
}  // namespace RType
