//
#ifndef SRC_GAME_LEVELMANAGER_HPP_
#define SRC_GAME_LEVELMANAGER_HPP_

#include "src/Config/LevelConfig.hpp"
#include "src/Config/LevelConfigResolver.hpp"
#include "src/Game/SceneManager.hpp"
#include <memory>
#include <rengine/src/Graphics/ASound.hpp>

namespace RType {
    /**
    * @addtogroup RType
    * @namespace RType
    * @class LevelManager
    * @brief Manage levels
    */
    class LevelManager
    {
        public:
            LevelManager(Rengine::ECS& ecs,SceneManager& sceneManager);
            ~LevelManager(void) = default;
            /**
            * @fn loadLevel
            * @brief Load a level from its path
            */
            bool loadLevel(const std::string& jsonPath);
            /**
            * @fn loadLevel
            * @param idx The index of the scene to be loaded
            * @return True if the loading was succesful, false otherwise
            * @brief Load the scene at index. Updates the ECS with the enemies and the backgrounds.
            */
            bool loadScene(uint8_t idx);
            /**
            * @fn updateDeltatime
            * @brief Update the deltatime
            */
            void updateDeltatime(void);
            /**
            * @fn isCurrentSceneOver
            * @return True if the current scene is over, false otherwise
            * @brief Check if the current scene is over.
            */
            bool isCurrentSceneOver(void);
            /**
            * @fn nextScene
            * @return True if the loading was succesful, false otherwise
            * @brief Load the next scene. Updates the ECS.
            */
            bool nextScene(void);
            /**
            * @fn previousScene
            * @return True if the loading was succesful, false otherwise
            * @brief Load the previous scene. Updates the ECS.
            */
            bool previousScene(void);
            /**
            * @fn resetCurrentScene
            * @return True if the loading was succesful, false otherwise
            * @brief Reload the current scene.
            */
            bool resetCurrentScene(void);
            /**
            * @fn getCurrentSceneEnemies
            * @return std::optional<const std::vector<RType::Config::SceneEntityConfig>> A vector of SceneEntityConfig
            * @brief Get the current scene's enemies.
            */
            std::optional<std::reference_wrapper<const std::vector<RType::Config::SceneEntityConfig>>> getCurrentSceneEnemies(void);
            /**
            * @fn getCurrentSceneBackgroundImages
            * @return std::optional<const std::vector<RType::Config::ImageConfig>> A vector of ImageConfig
            * @brief Get the current scene's background images.
            */
            std::optional<std::reference_wrapper<const std::vector<RType::Config::ImageConfig>>> getCurrentSceneBackgroundImages(void);
            void completeClear(void);

        private:
            void clearCurrentScene(void);

        private:
            Rengine::ECS& _ecs;
            float _time = 0;
            uint8_t _currentSceneIndex = 0;
            std::optional<std::reference_wrapper<const RType::Config::LevelConfig>> _levelConfig;
            std::optional<Rengine::Entity::size_type> _bossId;
            std::vector<Rengine::Entity::size_type> _currentSceneBackgroundEntities;
            std::vector<Rengine::Entity::size_type> _currentSceneEnemies;
            std::shared_ptr<Rengine::Graphics::ASound> _backgroundMusic = nullptr;
            SceneManager &_sceneManager;
    };

}  // namespace RType
#endif  // SRC_GAME_LEVELMANAGER_HPP_
