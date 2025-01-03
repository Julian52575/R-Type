//
#ifndef _SRC_CONFIG_LEVELCONFIG_HPP_
#define _SRC_CONFIG_LEVELCONFIG_HPP_
#include <exception>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <vector>

#include "EntityConfig.hpp"
#include "ImageConfig.hpp"

namespace RType {

    namespace Config {

        class LevelConfigExceptionFileError : public std::exception {
        public:
            LevelConfigExceptionFileError(const std::string& fileName, const char *jsonMsg)
            {
                std::string msg = "RType::Config::LevelConfig An error happened when trying to read from '" + fileName + "'.";

                this->_concat = msg + jsonMsg;
            };
            const char *what() const noexcept
            {
                return this->_concat.c_str();
            };

        private:
            std::string _concat;
        };
        class LevelConfigExceptionInvalidJsonFile : public std::exception {
            public:
                LevelConfigExceptionInvalidJsonFile(const std::string& fileName, const char *jsonMsg)
                {
                    std::string msg = "RType::Config::LevelConfig: Trying to construct from an invalid json file '" + fileName + "'.\nError: '";

                    this->_concat = msg + jsonMsg + "'.";
                };
                const char *what() const noexcept
                {
                    return this->_concat.c_str();
                };

            private:
                std::string _concat;
        };
        class LevelConfigExceptionConfigurationError : public std::exception {
            public:
                LevelConfigExceptionConfigurationError(std::string jsonMsg)
                {
                    std::string msg = "RType::Config::LevelConfig: Configuration Error: '";


                    this->_concat = msg + jsonMsg + "'.";
                };
                const char *what() const noexcept
                {
                    return this->_concat.c_str();
                };

            private:
                std::string _concat;
        };

        enum SceneEndCondition {
            SceneEndConditionNA,
            SceneEndConditionTime,
            SceneEndConditionBoss
        };  // enum SceneEndCondition
        union SceneEndConditionData {
            float time;
        };  // union SceneEndConditionData

        struct SceneEntityConfig {
            EntityConfig entityConfig;
            uint16_t xSpawn;
            uint16_t ySpawn;
            bool isBoss;
        };  // struct SceneEntityConfig

        class SceneConfig {
            public:
                SceneConfig(nlohmann::json& scene);
                ~SceneConfig(void) = default;

            public:
                SceneEndCondition _endCondition = SceneEndCondition::SceneEndConditionNA;
                SceneEndConditionData _endConditionData;
                float _scrollingSpeed;
                std::vector<RType::Config::ImageConfig> _backgroundImages;
                std::vector<RType::Config::SceneEntityConfig> _enemies;

            private:
                void parseBackground(nlohmann::json& backgroundField);
                void parseEnemies(nlohmann::json& enemiesField);
                void parseEndCondition(nlohmann::json& scene);

        };  // class SceneConfig

        class LevelConfig {
            public:
                LevelConfig(void) = default;
                LevelConfig(const std::string& jsonPath);
                ~LevelConfig(void) = default;
                const std::vector<RType::Config::SceneConfig>& getScenes(void) const noexcept;

            private:
                std::vector<RType::Config::SceneConfig> _scenes;

        };  // class LevelConfig
    }  // namespace Config
}  // namespace RType
#endif
