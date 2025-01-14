//
#include <climits>
#include <exception>
#include <filesystem>
#include <functional>
#include <optional>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Entity.hpp>
#include <rengine/src/Graphics/AWindow.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Rng.hpp>


#include "State.hpp"
#include "GameState.hpp"
#include "AState.hpp"


#include "src/Components/Clickable.hpp"
#include "src/Components/Metadata.hpp"
#include "src/Config/LevelConfigResolver.hpp"
#include "src/Components/Buff.hpp"
#include "src/Components/Sprite.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Action.hpp"
#include "src/Components/Components.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Relationship.hpp"
#include "src/Components/HitBoxViewer.hpp"

namespace RType {

    GameState::GameState(Rengine::ECS& ecs)
        : AState(ecs), _levelManager(ecs)
    {
        this->initScenes();
        this->_sceneManager.setScene(GameScenes::GameScenesLoadLevel);
    }

    void GameState::registerComponents(void)
    {
        // Component
        this->_ecs.registerComponent<RType::Components::Configuration>();
        this->_ecs.registerComponent<RType::Components::Action>();
        this->_ecs.registerComponent<RType::Components::Position>();
        this->_ecs.registerComponent<RType::Components::Sprite>();
        this->_ecs.registerComponent<Components::Buff>();
        this->_ecs.registerComponent<RType::Components::Hitbox>();
        this->_ecs.registerComponent<RType::Components::Relationship>();
        this->_ecs.registerComponent<RType::Components::Clickable>();
        this->_ecs.registerComponent<RType::Components::HitboxViewer>();
        this->_ecs.registerComponent<RType::Components::Metadata>();

        // Function
        this->_ecs.setComponentFunction<RType::Components::Sprite>(RType::Components::Sprite::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Action>(RType::Components::Action::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Position>(RType::Components::Position::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Hitbox>(RType::Components::Hitbox::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Clickable>(RType::Components::Clickable::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::HitboxViewer>(RType::Components::HitboxViewer::componentFunction);
    }

    void GameState::setNextLevelToLoad(const std::string& level)
    {
        this->_levelToLoad = level;
    }

    void GameState::loadLevel(const std::string& jsonPath)
    {
        this->_levelManager.loadLevel(jsonPath);
        this->createPlayer("assets/entities/skeletonDragon.json");
        this->loadCurrentLevelScene();
    }

    void GameState::loadCurrentLevelScene()
    {
        // background
        std::optional<std::reference_wrapper<const std::vector<RType::Config::ImageConfig>>> backgroundImages
            = this->_levelManager.getCurrentSceneBackgroundImages();
        // enemies
        std::optional<std::reference_wrapper<const std::vector<RType::Config::SceneEntityConfig>>> enemies
            = this->_levelManager.getCurrentSceneEnemies();
        uint64_t i = 0;

        if (backgroundImages.has_value() == false) {
            goto enemyLoading;
        }
        for (i = 0; i < backgroundImages->get().size(); i++) {
            auto sprite = Rengine::Graphics::GraphicManagerSingletone::get().createSprite(backgroundImages->get()[i].getSpecs());
            this->_backgroundSprites.push_back(sprite);
        }

enemyLoading:
        if (enemies.has_value() == false) {
            goto skipEnemyLoading;
        }
        this->_currentEnemies.clear();
        for (i = 0; i < enemies->get().size(); i++) {
            Rengine::Entity& currentEnemy = this->_ecs.addEntity();

            currentEnemy.addComponent<RType::Components::Position>(enemies->get()[i].xSpawn, enemies->get()[i].ySpawn);
            currentEnemy.addComponent<RType::Components::Sprite>(enemies->get()[i].entityConfig.getSprite().getSpecs());
            currentEnemy.addComponent<RType::Components::Hitbox>(enemies->get()[i].entityConfig.getHitbox());
            currentEnemy.addComponent<RType::Components::Configuration>(enemies->get()[i].entityConfig);
            currentEnemy.addComponent<RType::Components::HitboxViewer>(enemies->get()[i].entityConfig.getHitbox().size.x, enemies->get()[i].entityConfig.getHitbox().size.y);
            currentEnemy.addComponent<RType::Components::Relationship>();
            RType::Components::Metadata& meta = currentEnemy.addComponent<RType::Components::Metadata>();

            if (enemies->get()[i].isBoss == true) {
                meta.add(Components::Metadata::MetadataListBoss);
            }
            this->_currentEnemies.push_back(Rengine::Entity::size_type(currentEnemy));
        }
skipEnemyLoading:
        return;
    }

    State GameState::run(void)
    {
        if (this->_sceneManager.getCurrentScene() == GameScenes::GameScenesLoadLevel) {
            this->_sceneManager.callCurrentSceneFunction<void, GameState&>(*this);
            return State::StateGame;
        } else {
            return this->_sceneManager.callCurrentSceneFunction<State, GameState&>(*this);
        }
    }


    void GameState::deletePlayer(void)
    {
        // No player set
        if (this->_playerEntityId == RTYPE_NO_PLAYER_ENTITY_ID) {
            return;
        }
        try {
             this->_ecs.removeEntity(this->_playerEntityId);
        } catch (std::exception& e) {
            return;
        }
    }

    void loadLevelFunction(GameState& gameState)
    {
        // Simulate server by getting random level config
        std::vector<std::string> configVector;
        uint64_t idx = 0;

        for (auto file : std::filesystem::directory_iterator("assets/levels/")) {
            configVector.push_back(file.path());
        }
        if (configVector.size() == 0) {
            return;
        }
        idx = Rengine::rngFunction() % configVector.size();
        gameState.loadLevel(configVector[idx]);
        gameState._sceneManager.setScene(GameScenes::GameScenesPlay);
    }

    State playFunction(GameState& gameState)
    {
        gameState._levelManager.updateDeltatime(Rengine::Graphics::GraphicManagerSingletone::get().getWindow().get()->getDeltaTimeSeconds());
        if (gameState._levelManager.isCurrentSceneOver()) {
            if (!gameState._levelManager.nextScene()){
                gameState._sceneManager.setScene(GameScenes::GameScenesLoadLevel);
                return State::StateGame;
            }
            // detruire les entités courantes survivantes
            for (uint64_t i = 0; i < gameState._currentEnemies.size(); i++) {
                try {
                    gameState._ecs.removeEntity(gameState._currentEnemies[i]);
                } catch (std::exception& e) {
                    std::cerr << "Exception " << e.what() << "when removing entity " << gameState._currentEnemies[i] << std::endl;
                }
            }
            gameState.loadCurrentLevelScene();
        }

        gameState._ecs.runComponentFunction<RType::Components::Position>();  // move entity
        gameState._ecs.runComponentFunction<RType::Components::Action>();  // handle action player
        gameState._ecs.runComponentFunction<RType::Components::Hitbox>();  // handle collision
        for (int i = 0; i < gameState._backgroundSprites.size(); i++) {
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(gameState._backgroundSprites[i], {0, 0});
        }
        gameState._ecs.runComponentFunction<RType::Components::Sprite>();  // render sprite
        gameState._ecs.runComponentFunction<RType::Components::HitboxViewer>();  // render hitboxa
        gameState._ecs.runComponentFunction<RType::Components::Clickable>();  // check click on the few entity who has this component

        if (Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager()
        .receivedInput(Rengine::Graphics::UserInputTypeKeyboardSpecialPressed, {Rengine::Graphics::UserInputKeyboardSpecialESCAPE})) {
            return State::StateMenu;
        }
        return State::StateGame;
    }

    void GameState::initScenes(void)
    {
        // Laod level scene
        std::function<void(GameState&)> funScene1(loadLevelFunction);

        this->_sceneManager.setSceneFunction<void, GameState&>(GameScenes::GameScenesLoadLevel, funScene1);
        // Play scene
        std::function<State(GameState&)> funScene2(playFunction);

        this->_sceneManager.setSceneFunction<State, GameState&>(GameScenes::GameScenesPlay, funScene2);
    }

    void GameState::createPlayer(const std::string& jsonPath)
    {
        if (this->_playerEntityId != RTYPE_NO_PLAYER_ENTITY_ID) {
            this->deletePlayer();
        }
        Rengine::Entity& player = this->_ecs.addEntity();
        Config::EntityConfig enConfig(jsonPath);

        player.addComponent<RType::Components::Action>(this->_sceneManager, Components::ActionSourceUserInput);
        player.addComponent<RType::Components::Configuration>(enConfig);
        player.addComponent<RType::Components::Position>(0, 0);
        player.addComponent<RType::Components::Sprite>(enConfig.getSprite().getSpecs());
        player.addComponent<RType::Components::Buff>();
        player.addComponent<RType::Components::Hitbox>(enConfig.getHitbox());
        player.addComponent<RType::Components::Clickable>( [](void){} );  // damn fork bomb is an empty lambda
        Components::Relationship& relationship = player.addComponent<Components::Relationship>();

        player.addComponent<RType::Components::HitboxViewer>(enConfig.getHitbox().size.x, enConfig.getHitbox().size.y);

        this->_playerEntityId = Rengine::Entity::size_type(player);
    }

    void GameState::alertPlayer(void)
    {
        Rengine::Entity& playerEntity = this->_ecs.getEntity(this->_playerEntityId);
        RType::Components::Position& playerPos = playerEntity.getComponent<RType::Components::Position>();
        Rengine::SparseArray<RType::Components::Position>& spPosition = this->_ecs.getComponents<RType::Components::Position>();
        Rengine::SparseArray<RType::Components::Relationship>& spRelationship = this->_ecs.getComponents<RType::Components::Relationship>();

        for (Rengine::ECS::size_type index = 0; index < this->_ecs.getEntityLimit(); index++) {
            if (index == this->_playerEntityId) {
                continue;
            }
            if (spRelationship[index].has_value() == false) {
                continue;
            }
            if (spRelationship[index]->isParented(uint64_t(this->_playerEntityId))) {
                continue;
            }
            if (spPosition[index].has_value() == false) {
                continue;
            }
            Rengine::Graphics::vector2D<float> pos = spPosition[index]->getVector2D();
            float distance = sqrt(pow(playerPos.getVector2D().x - pos.x, 2) + pow(playerPos.getVector2D().y - pos.y, 2));

            if (distance > 200) {
                continue;
            }
            std::cout << "Alert between " << int(this->_playerEntityId) << " and " << index << std::endl;
            #warning Debug print
        }
    }

}  // namespace RType
