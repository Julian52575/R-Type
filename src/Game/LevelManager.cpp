//
#include <exception>
#include <functional>
#include <optional>
#include <rengine/src/ECS.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Entity.hpp>

#include "LevelManager.hpp"
#include "src/Game/EntityMaker.hpp"
#include "src/Config/LevelConfig.hpp"
#include "src/Config/LevelConfigResolver.hpp"
#include "src/Components/Metadata.hpp"
#include "src/Components/Buff.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Action.hpp"
#include "src/Components/Components.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Relationship.hpp"
#include "src/Components/Chrono.hpp"
#include "src/Components/Life.hpp"
#include "Team.hpp"

namespace RType {

    LevelManager::LevelManager(std::shared_ptr<Rengine::ECS> ecs) : _ecs(ecs)
    {
    }

    void LevelManager::setECS(std::shared_ptr<Rengine::ECS> ecs)
    {
        this->_ecs = ecs;
    }

    bool LevelManager::loadLevel(const std::string& jsonPath)
    {
        RType::Config::LevelConfigResolver& resolver = RType::Config::LevelConfigResolverSingletone::get();
        try {
            this->_levelConfig = resolver.get(jsonPath);
        } catch (std::exception& e) {
            return false;
        }
        this->_levelName = jsonPath;
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
        // enemies
        std::optional<std::reference_wrapper<const std::vector<RType::Config::SceneEntityConfig>>> enemies
            = this->getCurrentSceneEnemies();
        uint64_t i = 0;

enemyLoading:
        if (enemies.has_value() == false) {
            goto loadSceneReturn;
        }
        for (i = 0; i < enemies->get().size(); i++) {
            Rengine::Entity& currentEnemy = RType::EntityMaker::make(this->_ecs, enemies->get()[i].entityConfig, Team::TeamEnemy);

            currentEnemy.getComponent<RType::Components::Position>().set(
                    {(float) enemies->get()[i].xSpawn,
                    (float) enemies->get()[i].ySpawn}
            );
            currentEnemy.addComponent<RType::Components::Action>(RType::Components::ActionSourceScript, enemies->get()[i].scriptPath);

            if (enemies->get()[i].isBoss == true) {
                currentEnemy.getComponent<RType::Components::Metadata>().add(RType::Components::Metadata::MetadataListBoss);
                this->_bossId = Rengine::Entity::size_type(currentEnemy);
            }
            RType::Components::Relationship& rel = currentEnemy.getComponent<RType::Components::Relationship>();

            rel.setGroup(Team::TeamEnemy);
            currentEnemy.setComponentsDestroyFunction(
                [this](Rengine::Entity& en) {
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

    void LevelManager::completeClear(void)
    {
        this->clearCurrentScene();
        this->_levelConfig = std::nullopt;
    }

    void LevelManager::clearCurrentScene(void)
    {
        this->_bossId = std::nullopt;
        this->_time = 0;
        for (auto enIt : this->_currentSceneEnemies) {
            try {
                this->_ecs->removeEntity<>(enIt);
            } catch (std::exception& e) {;}
        }
    }
    const std::string& LevelManager::getLevelName(void) const noexcept
    {
        return this->_levelName;
    }

}  // namespace RType
