//
#include <exception>
#include <functional>
#include <optional>
#include <rengine/src/ECS.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Entity.hpp>
#include <rengine/src/Graphics/ASprite.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>

#include "LevelManager.hpp"
#include "src/Game/EntityMaker.hpp"
#include "src/Config/LevelConfig.hpp"
#include "src/Config/LevelConfigResolver.hpp"
#include "src/Components/Clickable.hpp"
#include "src/Components/Metadata.hpp"
#include "src/Components/Buff.hpp"
#include "src/Components/Sprite.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Action.hpp"
#include "src/Components/Components.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Relationship.hpp"

#ifdef DEBUG
    #include "src/Components/HitboxViewer.hpp"
    #include "src/Components/HealthViewer.hpp"
#endif
#include "src/Components/Chrono.hpp"
#include "src/Components/Life.hpp"
#include "Team.hpp"

namespace RType {

    LevelManager::LevelManager(Rengine::ECS& ecs,SceneManager& sceneManager) : _ecs(ecs), _sceneManager(sceneManager)
    {
    }

    bool LevelManager::loadLevel(const std::string& jsonPath)
    {
        RType::Config::LevelConfigResolver& resolver = RType::Config::LevelConfigResolverSingletone::get();
        try {
            this->_levelConfig = resolver.get(jsonPath);
        } catch (std::exception& e) {
            return false;
        }
        return this->loadScene(0);
    }

    bool LevelManager::loadScene(uint8_t index)
    {
        this->clearCurrentScene();
        if (this->_levelConfig.has_value() == false) {
            return false;
        }
        if (index >= this->_levelConfig->get().getScenes().size()) {
            return false;
        }
        this->_currentSceneIndex = index;
        this->_time = 0;
        // BGM
        if (this->_levelConfig->get().getScenes()[this->_currentSceneIndex].backgroundMusic != "") {
            Rengine::Graphics::SoundSpecs music;
            music.soundPath = this->_levelConfig->get().getScenes()[this->_currentSceneIndex].backgroundMusic;
            music.loop = true;
            try {
                if (this->_backgroundMusic != nullptr) {
                    this->_backgroundMusic->reset();
                }
                this->_backgroundMusic = Rengine::Graphics::GraphicManagerSingletone::get().createSound(music);
                this->_backgroundMusic->play();
            } catch (std::exception& e) {
                std::cerr << "Cannot load music " << this->_levelConfig->get().getScenes()[this->_currentSceneIndex].backgroundMusic << std::endl;
                this->_backgroundMusic = nullptr;
            }
        }

       // background
        std::optional<std::reference_wrapper<const std::vector<RType::Config::ImageConfig>>> backgroundImages
            = this->getCurrentSceneBackgroundImages();
        uint64_t i = 0;

        // No background : skip background
        if (backgroundImages.has_value() == false) {
            goto loadSceneReturn;
        }
        // No entity left : skip background
        if ((this->_ecs.getEntityLimit() - this->_ecs.getActiveEntitiesCount()) < backgroundImages->get().size()) {
            goto loadSceneReturn;
        }
        for (i = 0; i < backgroundImages->get().size(); i++) {
            this->_currentSceneBackgroundVector.emplace_back(
                std::move(
                    Rengine::Graphics::GraphicManagerSingletone::get().createSprite(backgroundImages->get()[i].getSpecs())
                )
            );
        }
loadSceneReturn:
        return true;
    }

    void LevelManager::updateDeltatime(void)
    {
        this->_time += Rengine::Clock::getElapsedTime();
    }

    void LevelManager::renderBackground(void)
    {
        for (auto it : this->_currentSceneBackgroundVector) {
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(
                it,
                {0, 0}
            );
        }
    }

    bool LevelManager::isCurrentSceneOver(void)
    {
        // No level config : return true and hope for reload
        if (this->_levelConfig.has_value() == false) {
            return true;
        }

        switch (this->_levelConfig->get().getScenes()[this->_currentSceneIndex % this->_levelConfig->get().getScenes().size()].endCondition) {
            case (Config::SceneEndConditionTime):
                if (this->_time >= this->_levelConfig->get().getScenes()[this->_currentSceneIndex % this->_levelConfig->get().getScenes().size()].endConditionData.time) {
                    return true;
                }
                break;

            case (Config::SceneEndConditionBossDefeat):
                if (this->_bossId.has_value() == false) {
                    return true;
                }
                break;

            // Invalid end condition : return true and hope for reload
            default:
                return true;
        }
        return false;
    }

    bool LevelManager::nextScene(void)
    {
        return this->loadScene(this->_currentSceneIndex + 1);
    }

    bool LevelManager::previousScene(void)
    {
        return this->loadScene(this->_currentSceneIndex - 1);
    }

    bool LevelManager::resetCurrentScene(void)
    {
        return this->loadScene(this->_currentSceneIndex);
    }

    std::optional<std::reference_wrapper<const std::vector<RType::Config::ImageConfig>>> LevelManager::getCurrentSceneBackgroundImages(void)
    {
        if (this->_levelConfig.has_value() == false) {
            return std::nullopt;
        }
        return this->_levelConfig->get().getScenes()[this->_currentSceneIndex % this->_levelConfig->get().getScenes().size()].backgroundImages;
    }

    void LevelManager::completeClear(void)
    {
        this->clearCurrentScene();
        this->_levelConfig = std::nullopt;
    }

    void LevelManager::clearCurrentScene(void)
    {
        this->_backgroundMusic = nullptr;
        this->_currentSceneBackgroundVector.clear();
        this->_bossId = std::nullopt;
        this->_time = 0;
    }
}  // namespace RType
