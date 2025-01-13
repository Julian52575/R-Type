//
#include <exception>
#include <nlohmann/json.hpp>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <utility>
#include <vector>

#include "EntityConfig.hpp"
#include "EntityConfigResolver.hpp"
#include "ImageConfigResolver.hpp"
#include "LevelConfig.hpp"
#include "LevelConfigResolver.hpp"
#include "src/Config/ImageConfig.hpp"

namespace RType {

    namespace Config {

        SceneConfig::SceneConfig(nlohmann::json& scene)
        {
            if (scene.contains("scrollingSpeed") == true) {
                this->scrollingSpeed = scene["scrollingSpeed"];
            } else {
                this->scrollingSpeed = 10.0f;
            }
            // Background
            this->parseBackground(scene["background"]);
            // Enemies
            this->parseEnemies(scene["enemies"]);
            // EndCondition
            this->parseEndCondition(scene);
        }

        void SceneConfig::parseBackground(nlohmann::json& backgroundField)
        {
            RType::Config::ImageConfigResolver& imageResolver = RType::Config::ImageConfigResolverSingletone::get();

            for (auto it : backgroundField) {
                // Not a string ?
                if (it.type() != nlohmann::json::value_t::string) {
                    throw std::runtime_error("Background value is not a string.");
                }
                try {
                    std::string jsonPath = it;
                    const ImageConfig& currentImageConfig = imageResolver.get(jsonPath);

                    this->backgroundImages.emplace_back(currentImageConfig);
                } catch (std::exception& e) {
                    std::string err = e.what();
                    std::string msg = err + " on background image '" + (std::string) it + "'.";

                    throw std::runtime_error(msg);
                }
            }
        }
        void SceneConfig::parseEnemies(nlohmann::json& enemiesField)
        {
            // Not an array ?
            if (enemiesField.type() != nlohmann::json::value_t::array) {
                throw std::runtime_error("'enemies' field is not an array.");
            }
            RType::Config::EntityConfigResolverSingletone entitySingletone;
            RType::Config::EntityConfigResolver& entityResolver = entitySingletone.get();

            for (auto it : enemiesField) {
                if (it.contains("json") == false) {
                    throw std::runtime_error("No 'json' field in one of 'enemies'.");
                }
                if (it.contains("x") == false) {
                    throw std::runtime_error("No 'x' field in one of 'enemies'.");
                }
                if (it.contains("y") == false) {
                    throw std::runtime_error("No 'y' field in one of 'enemies'.");
                }
                RType::Config::SceneEntityConfig config;
                std::string jsonPath = it["json"];
                const RType::Config::EntityConfig& entityConfigRef = entityResolver.get(jsonPath);

                config.entityConfig = std::move(entityConfigRef);
                config.xSpawn = it["x"];
                config.ySpawn = it["y"];
                // Boss
                if (it.contains("boss") == true) {
                    config.isBoss = it["boss"];
                }
                this->enemies.push_back(config);
            }
        }

        void SceneConfig::parseEndCondition(nlohmann::json& scene)
        {
            std::vector<std::pair<std::string, SceneEndCondition>> endCond = {
                {"time", SceneEndConditionTime},
                {"bossDefeat", SceneEndConditionBossDefeat}
            };
            std::string condStr = scene["endCondition"];
            auto it = endCond.begin();
            /* Convert str to Enum */
            while (it != endCond.end()) {
                if (it->first == condStr) {
                    this->endCondition = it->second;
                    break;
                }
                it++;
            }
            if (it == endCond.end()) {
                throw LevelConfigExceptionConfigurationError("Unknow 'endCondition': " + condStr + "'.");
            }
            /* Enum Unique field */
            uint8_t bossCount = 0;

            switch (this->endCondition) {
                case SceneEndConditionTime:
                    this->endConditionData.time = scene["endConditionTime"];
                    break;

                case SceneEndConditionBossDefeat:
                    // check Boss enemy
                    if (this->enemies.size() == 0) {
                        throw LevelConfigExceptionConfigurationError("No 'enemy' in scene of 'endCondition' = 'bossDefeat'.");
                    }
                    for (auto sceneIt : this->enemies) {
                        if (sceneIt.isBoss == true) {
                            bossCount += 1;
                            if (bossCount >= 2) {
                                throw LevelConfigExceptionConfigurationError("More than 1 enemy marked as 'boss'.");
                            }
                        }
                    }  // for
                    if (bossCount == 0) {
                        throw LevelConfigExceptionConfigurationError("No 'boss' = true enemy on 'endCondition' = 'bossDefeat'.");
                    }
                    break;

                default:
                    break;
            }
        }

        LevelConfig::LevelConfig(const std::string& jsonPath)
        {
            std::ifstream f;

            try {
                f.open(jsonPath);
            } catch (std::exception &e) {
                throw LevelConfigExceptionFileError(jsonPath, e.what());
            }
            nlohmann::json j;

            try {
                j = nlohmann::json::parse(f);
            } catch (std::exception &e) {
                throw LevelConfigExceptionInvalidJsonFile(jsonPath, e.what());
            }
            // level check
            if (j.contains("level") == false) {
                throw LevelConfigExceptionInvalidJsonFile(jsonPath, "No 'level' field.");
            }
            j = j["level"];
            // scenes check
            if (j.contains("scenes") == false) {
                throw LevelConfigExceptionInvalidJsonFile(jsonPath, "No 'scenes' field.");
            }
            if (j["scenes"].type() != nlohmann::json::value_t::array) {
                throw LevelConfigExceptionInvalidJsonFile(jsonPath, "'scenes' field is not an array.");
            }
            if (j["scenes"].size() == 0) {
                throw LevelConfigExceptionInvalidJsonFile(jsonPath, "'scenes' field is empty.");
            }
            // scenes parsing
            int count = 0;

            try {
                for (auto it : j["scenes"]) {
                    count += 1;
                    SceneConfig currentScene = SceneConfig(it);
                    this->_scenes.push_back(currentScene);
                }
            } catch (std::exception& e) {
                std::string err = e.what();
                std::string msg = "Error on scene " + std::to_string(count) + " : " + err;

                LevelConfigExceptionInvalidJsonFile(jsonPath, msg.c_str());
            }
        }

        const std::vector<RType::Config::SceneConfig>& LevelConfig::getScenes(void) const noexcept
        {
            return this->_scenes;
        }

    }  // namespace Config
}  // namespace RType
