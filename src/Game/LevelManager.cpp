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
#include "src/Components/Clickable.hpp"
#include "src/Components/Metadata.hpp"
#include "src/Components/Buff.hpp"
#include "src/Components/Sprite.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Action.hpp"
#include "src/Components/Components.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Relationship.hpp"
#include "src/Components/HitboxViewer.hpp"
#include "src/Components/HealthViewer.hpp"
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

       // background
        std::optional<std::reference_wrapper<const std::vector<RType::Config::ImageConfig>>> backgroundImages
            = this->getCurrentSceneBackgroundImages();
        // enemies
        std::optional<std::reference_wrapper<const std::vector<RType::Config::SceneEntityConfig>>> enemies
            = this->getCurrentSceneEnemies();
        uint64_t i = 0;

        // No background : skip background
        if (backgroundImages.has_value() == false) {
            goto enemyLoading;
        }
        // No entity left : skip background
        if ((this->_ecs.getEntityLimit() - this->_ecs.getActiveEntitiesCount()) < backgroundImages->get().size()) {
            goto enemyLoading;
        }
        for (i = 0; i < backgroundImages->get().size(); i++) {
            Rengine::Entity& bgEntity = this->_ecs.addEntity();

            bgEntity.addComponent<RType::Components::Position>(0, 0);
            bgEntity.addComponent<RType::Components::Sprite>(backgroundImages->get()[i].getSpecs());
            bgEntity.setComponentsDestroyFunction(
                [](Rengine::Entity& en) {
                    en.removeComponent<RType::Components::Position>();
                    en.removeComponent<RType::Components::Sprite>();
                }
            );
            this->_currentSceneBackgroundEntities.push_back(Rengine::ECS::size_type(bgEntity));
        }

enemyLoading:
        if (enemies.has_value() == false) {
            goto loadSceneReturn;
        }
        for (i = 0; i < enemies->get().size(); i++) {
            Rengine::Entity& currentEnemy = this->_ecs.addEntity();

            currentEnemy.addComponent<RType::Components::Position>(enemies->get()[i].xSpawn, enemies->get()[i].ySpawn);
            currentEnemy.addComponent<RType::Components::Sprite>(enemies->get()[i].entityConfig.getSprite().getSpecs());
            currentEnemy.getComponent<RType::Components::Sprite>().getSprite().get()->flip();
            currentEnemy.addComponent<RType::Components::Hitbox>(enemies->get()[i].entityConfig.getHitbox());
            currentEnemy.addComponent<RType::Components::Configuration>(enemies->get()[i].entityConfig);
            currentEnemy.addComponent<RType::Components::HitboxViewer>(enemies->get()[i].entityConfig.getHitbox().size.x, enemies->get()[i].entityConfig.getHitbox().size.y);
            currentEnemy.addComponent<RType::Components::Life>(enemies->get()[i].entityConfig.getStats().hp);
            currentEnemy.addComponent<RType::Components::HealthViewer>(enemies->get()[i].entityConfig.getStats().hp);
            RType::Components::Relationship& rel = currentEnemy.addComponent<RType::Components::Relationship>();
            RType::Components::Metadata& meta = currentEnemy.addComponent<RType::Components::Metadata>();

            // std::cout << "Script: " << enemies->get()[i].scriptPath << std::endl;
            currentEnemy.addComponent<RType::Components::Action>(RType::Components::ActionSourceScript, enemies->get()[i].scriptPath);

            if (enemies->get()[i].isBoss == true) {
                meta.add(RType::Components::Metadata::MetadataListBoss);
                this->_bossId = Rengine::Entity::size_type(currentEnemy);
            }
            rel.setGroup(Team::TeamEnemy);
            currentEnemy.setComponentsDestroyFunction(
                [this](Rengine::Entity& en) {
                    en.removeComponentNoExcept<RType::Components::Position>();
                    en.removeComponentNoExcept<RType::Components::Sprite>();
                    en.removeComponentNoExcept<RType::Components::Hitbox>();
                    en.removeComponentNoExcept<RType::Components::Configuration>();
                    en.removeComponentNoExcept<RType::Components::HitboxViewer>();
                    en.removeComponentNoExcept<RType::Components::Relationship>();
                    en.removeComponentNoExcept<RType::Components::Life>();
                    en.removeComponentNoExcept<RType::Components::HealthViewer>();
                    en.removeComponentNoExcept<RType::Components::Metadata>();
                    en.removeComponentNoExcept<RType::Components::Action>();

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
                this->_ecs.removeEntity(bgIt);
            } catch (std::exception& e) {;}
        }
        for (auto enIt : this->_currentSceneEnemies) {
            try {
                this->_ecs.removeEntity(enIt);
            } catch (std::exception& e) {;}
        }
    }

}  // namespace RType
