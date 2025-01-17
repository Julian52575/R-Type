//
#include <exception>
#include <functional>
#include <optional>
#include <rengine/src/ECS.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Entity.hpp>

#include "LevelManager.hpp"
#include "src/Config/LevelConfig.hpp"
#include "src/Config/LevelConfigResolver.hpp"
#include "src/Components/Metadata.hpp"
#include "src/Components/Buff.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Components.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Relationship.hpp"
#include "src/Components/Chrono.hpp"
#include "src/Components/Life.hpp"

namespace RType {

    LevelManager::LevelManager(std::shared_ptr<Rengine::ECS> ecs)
        : _ecs(ecs)
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

    void LevelManager::setECS(std::shared_ptr<Rengine::ECS> ecs)
    {
        this->_ecs = ecs;
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

        // enemies
        std::optional<std::reference_wrapper<const std::vector<RType::Config::SceneEntityConfig>>> enemies
            = this->getCurrentSceneEnemies();
        uint64_t i = 0;

enemyLoading:
        if (enemies.has_value() == false) {
            goto loadSceneReturn;
        }
        for (i = 0; i < enemies->get().size(); i++) {
            Rengine::Entity& currentEnemy = this->_ecs->addEntity();

            currentEnemy.addComponent<RType::Components::Position>(enemies->get()[i].xSpawn, enemies->get()[i].ySpawn);

            currentEnemy.addComponent<RType::Components::Hitbox>(enemies->get()[i].entityConfig.getHitbox());
            currentEnemy.addComponent<RType::Components::Configuration>(enemies->get()[i].entityConfig);
            currentEnemy.addComponent<RType::Components::Relationship>();
            
            currentEnemy.addComponent<RType::Components::Life>(enemies->get()[i].entityConfig.getStats().hp);

            RType::Components::Metadata& meta = currentEnemy.addComponent<RType::Components::Metadata>();

            if (enemies->get()[i].isBoss == true) {
                meta.add(RType::Components::Metadata::MetadataListBoss);
                this->_bossId = Rengine::Entity::size_type(currentEnemy);
            }

            currentEnemy.setComponentsDestroyFunction(
                [this](Rengine::Entity& en) {
                    en.removeComponent<RType::Components::Position>();
                    en.removeComponent<RType::Components::Hitbox>();
                    en.removeComponent<RType::Components::Configuration>();
                    en.removeComponent<RType::Components::Relationship>();
                    en.removeComponent<RType::Components::Metadata>();
                    en.removeComponent<RType::Components::Life>();

                    if (this->_bossId.has_value() == true && this->_bossId.value() == Rengine::Entity::size_type(en)) {
                        this->_bossId = std::nullopt;
                    }
                    
                }
            );
            this->_currentSceneEnemies.push_back(Rengine::Entity::size_type(currentEnemy));
        }
loadSceneReturn:
        return true;
    }

    void LevelManager::updateDeltatime(void)
    {
        this->_time += Rengine::Clock::getElapsedTime();
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

    std::optional<std::reference_wrapper<const std::vector<RType::Config::SceneEntityConfig>>> LevelManager::getCurrentSceneEnemies(void)
    {
        if (this->_levelConfig.has_value() == false) {
            return std::nullopt;
        }
        return this->_levelConfig->get().getScenes()[this->_currentSceneIndex % this->_levelConfig->get().getScenes().size()].enemies;
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
        this->_currentSceneBackgroundEntities.clear();
        this->_bossId = std::nullopt;
        this->_time = 0;
        for (auto bgIt : this->_currentSceneBackgroundEntities) {
            try {
                this->_ecs->removeEntity(bgIt);
            } catch (std::exception& e) {;}
        }
        for (auto enIt : this->_currentSceneEnemies) {
            try {
                this->_ecs->removeEntity(enIt);
            } catch (std::exception& e) {;}
        }
    }

}  // namespace RType
