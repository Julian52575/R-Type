//
#include <exception>
#include <filesystem>
#include <functional>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Entity.hpp>
#include <rengine/src/Graphics/AWindow.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Rng.hpp>

#include "src/Components/Buff.hpp"
#include "src/Components/Configuration.hpp"
#include "src/Components/Action.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Sprite.hpp"
#include "src/Config/LevelConfigResolver.hpp"
#include "State.hpp"
#include "GameState.hpp"
#include "AState.hpp"

namespace RType {

    GameState::GameState(Rengine::ECS& ecs) : AState(ecs)
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
        // Function
        this->_ecs.setComponentFunction<RType::Components::Sprite>(RType::Components::Sprite::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Action>(RType::Components::Action::componentFunction);
    }

    void GameState::setNextLevelToLoad(const std::string& level)
    {
        this->_levelToLoad = level;
    }

    void GameState::loadLevel(const std::string& jsonPath)
    {
        RType::Config::LevelConfigResolver& resolver = RType::Config::LevelConfigResolverSingletone::get();
        RType::Config::LevelConfig& config = resolver.get(jsonPath);

        this->loadLevel(config);
    }

    void GameState::loadLevel(const RType::Config::LevelConfig& levelConfig)
    {
        this->createPlayer("assets/entities/playerDefault.json");
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

    void GameState::createPlayer(const std::string& jsonPath)
    {
        if (this->_playerEntityId != RTYPE_NO_PLAYER_ENTITY_ID) {
            this->deletePlayer();
        }
        Rengine::Entity& player = this->_ecs.addEntity();
        Config::EntityConfig enConfig(jsonPath);

        player.addComponent<Components::Action>(this->_sceneManager, Components::ActionSourceUserInput);
        player.addComponent<Components::Configuration>(enConfig);
        player.addComponent<Components::Position>(0, 0);
        player.addComponent<Components::Sprite>(enConfig.getSprite().getSpecs());
        player.addComponent<Components::Buff>();
        this->_playerEntityId = Rengine::Entity::size_type(player);
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

    void GameState::sendInputToPlayerEntity(void)
    {
        if (this->_playerEntityId == RTYPE_NO_PLAYER_ENTITY_ID) {
            return;
        }
        try {
            Rengine::Entity& playerEntity = this->_ecs.getEntity(this->_playerEntityId);
            RType::Components::Action& actionComponent = playerEntity.getComponent<RType::Components::Action>();

            actionComponent.processUserInput();
        } catch (Rengine::EntityExceptionComponentNotLinked& e) {
            // No Action component
            return;
        } catch (Rengine::ECSExceptionEntityNotFound& e) {
            // No entity at this->_playerEntityId
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
        gameState.sendInputToPlayerEntity();
        gameState._ecs.runComponentFunction<Components::Action>();
        gameState._ecs.runComponentFunction<RType::Components::Sprite>();
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

}  // namespace RType
