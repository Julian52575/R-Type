//
#include <exception>
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "EntityConfig.hpp"
#include "LevelConfig.hpp"

namespace RType {

    namespace Config {

        SceneConfig::SceneConfig(nlohmann::json& scene)
        {
            this->scrollingSpeed = scene["scrollingSpeed"];
            // Background
            try {
                this->parseBackground(scene["background"]);
            } catch (std::exception& e) {
                std::string msg = e.what();

                throw std::runtime_error("Error when parsing background: " + msg);
            }
            // Enemies
            try {
                this->parseEnemies(scene["enemies"]);
            } catch (std::exception& e) {
                std::string msg = e.what();

                throw std::runtime_error("Error when parsing enemies: " + msg);
            }
            // EndCondition
            try {
                this->parseEndCondition(scene);
            } catch (std::exception& e) {
                std::string msg = e.what();

                throw std::runtime_error("Error when parsing end condition: " + msg);
            }
        }

        void SceneConfig::parseBackground(nlohmann::json& backgroundField)
        {
            for (auto it : backgroundField) {
                this->backgroundImages.push_back(RType::Config::ImageConfig(it));
            }
        }
        void SceneConfig::parseEnemies(nlohmann::json& enemiesField)
        {
            for (auto it : enemiesField) {
                RType::Config::SceneEntityConfig config;

                config.entityConfig = RType::Config::EntityConfig(it["json"]);
                config.xSpawn = it["x"];
                config.ySpawn = it["y"];
                // Boss
                try {
                    config.isBoss = it["boss"];
                } catch (std::exception& e) {
                    ;  //Ignore boss not found. damn bro that's crazy
                }
                this->enemies.push_back(config);
            }
        }

        void SceneConfig::parseEndCondition(nlohmann::json& scene)
        {
            std::vector<std::pair<std::string, SceneEndCondition>> endCond = {
                {"time", SceneEndConditionTime},
                {"bossDefeat", SceneEndConditionBoss}
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

                case SceneEndConditionBoss:
                    // check Boss enemy
                    for (auto it : this->enemies) {
                        if (it.isBoss == true) {
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
                j = j["level"];
                for (auto it : j["scenes"]) {
                    SceneConfig config(it);

                    this->_scenes.push_back(config);
                }
                if (this->_scenes.size() == 0) {
                    throw std::runtime_error("Error: no scenes.");
                }
            }
            // Error on parsing
            catch (std::exception &e) {
                throw LevelConfigExceptionInvalidJsonFile(jsonPath, e.what());
            }
        }

        const std::vector<RType::Config::SceneConfig>& LevelConfig::getScenes(void) const noexcept
        {
            return this->_scenes;
        }

    }  // namespace Config
}  // namespace RType
