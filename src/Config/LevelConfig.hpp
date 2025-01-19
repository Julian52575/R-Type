//
#ifndef SRC_CONFIG_LEVELCONFIG_HPP_
#define SRC_CONFIG_LEVELCONFIG_HPP_
#include <exception>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <vector>
#include <string>

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
        explicit LevelConfigExceptionConfigurationError(std::string jsonMsg)
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
    SceneEndConditionBossDefeat
};  // enum SceneEndCondition

union SceneEndConditionData {
    float time;
};  // union SceneEndConditionData

/**
* @addtogroup RType::Config
* @namespace Config
* @class SceneEntityConfig
* @brief A structure containing the data for a single entity in a single scene.
*/
struct SceneEntityConfig {
    EntityConfig entityConfig;
    std::string path;
    uint16_t xSpawn = 1919;
    uint16_t ySpawn = 0;
    std::string scriptPath = "";
    bool isBoss = false;
};  // struct SceneEntityConfig

/**
* @addtogroup RType::Config
* @namespace Config
* @class SceneConfig
* @brief This class contains the configuration data for a single scene.
*/
class SceneConfig {
    public:
        SceneConfig(void) = default;
        explicit SceneConfig(nlohmann::json& scene);
        ~SceneConfig(void) = default;

    public:
        /**
         * @brief The end condition for the scene. See RType::Config::SceneEndCondition.
        */
        SceneEndCondition endCondition = SceneEndCondition::SceneEndConditionNA;
        /**
         * @brief An union whose field are updated depending on this->_endCondition.
        */
        SceneEndConditionData endConditionData;
        /**
         * @brief The scene's scrollingSpeed.
        */
        float scrollingSpeed = 10;
        /**
        * @brief The scene's background music.
        */
        std::string backgroundMusic = "";
        /**
         * @brief A vector of ImageConfig for the background images.
        */
        std::vector<RType::Config::ImageConfig> backgroundImages;
        /**
         * @brief A vector of SceneEntityConfig for the scene's enemy.
        */
        std::vector<RType::Config::SceneEntityConfig> enemies;

    private:
        void parseBackground(nlohmann::json& backgroundField);
        void parseEnemies(nlohmann::json& enemiesField);
        void parseEndCondition(nlohmann::json& scene);
};  // class SceneConfig
/**
* @addtogroup RType::Config
* @namespace Config
* @class LevelConfig
* @brief This class encapsulate the level json.
* Is used to convert a jsonPath to a class of c++ variable.
*/
class LevelConfig {
    public:
        /**
        * @fn LevelConfig
        * @brief Create an empty LevelConfig.
        */
        LevelConfig(void) = default;
        /**
        * @fn LevelConfig
        * @param jsonPath The path to a json level configuration.
        * The image configuration must start with the 'level' field.
        */
        explicit LevelConfig(const std::string& jsonPath);
        ~LevelConfig(void) = default;
        /**
        * @fn getScenes
        * @return std::vector<SceneConfig> A vector of SceneConfig.
        * @brief Return the vector of SceneConfig.
        */
        const std::vector<RType::Config::SceneConfig>& getScenes(void) const;
    private:
        std::vector<RType::Config::SceneConfig> _scenes;
};  // class LevelConfig
}  // namespace Config
}  // namespace RType
#endif  // SRC_CONFIG_LEVELCONFIG_HPP_
