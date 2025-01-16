//
#include <climits>
#include <exception>
#include <filesystem>
#include <functional>
#include <optional>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>

#include "State.hpp"
#include "GameState.hpp"
#include "AState.hpp"
#include "src/Config/LevelConfigResolver.hpp"
#include "src/Components/Clickable.hpp"
#include "src/Components/Configuration.hpp"
#include "src/Components/Velocity.hpp"
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
        this->_ecs.registerComponent<RType::Components::Velocity>();
        this->_ecs.registerComponent<RType::Components::HealthViewer>();
        this->_ecs.registerComponent<RType::Components::Chrono>();
        this->_ecs.registerComponent<RType::Components::Life>();

        // Function
        this->_ecs.setComponentFunction<RType::Components::Sprite>(RType::Components::Sprite::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Action>(RType::Components::Action::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Velocity>(RType::Components::Velocity::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Hitbox>(RType::Components::Hitbox::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Clickable>(RType::Components::Clickable::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::HitboxViewer>(RType::Components::HitboxViewer::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::HealthViewer>(RType::Components::HealthViewer::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Chrono>(RType::Components::Chrono::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Life>(RType::Components::Life::componentFunction);
    }

    void GameState::loadLevel(const std::string& jsonPath)
    {
        this->_levelManager.loadLevel(jsonPath);
        this->createPlayer("assets/entities/skeletonDragon.json");
    }


    State GameState::run(void)
    {
        State s = this->_sceneManager.callCurrentSceneFunction<State, GameState&>(*this);

        return s;
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

    State loadLevelFunction(GameState& gameState)
    {
        // Simulate server by getting random level config
        std::vector<std::string> configVector;
        uint64_t idx = 0;

        for (auto file : std::filesystem::directory_iterator("assets/levels/")) {
            configVector.push_back(file.path());
        }
        if (configVector.size() == 0) {
            return State::StateMenu;
        }
        idx = Rengine::RNG::rngFunction() % configVector.size();
        gameState.loadLevel(configVector[idx]);
        gameState._sceneManager.setScene(GameScenes::GameScenesPlay);
        return State::StateGame;
    }

    State playFunction(GameState& gameState)
    {
        gameState._levelManager.updateDeltatime();
        if (gameState._levelManager.isCurrentSceneOver()) {
            if (!gameState._levelManager.nextScene()) {
                gameState._sceneManager.setScene(GameScenes::GameScenesLoadLevel);
                std::cout << "Level finished !" << std::endl;
                return State::StateMenu;
            }
        }
        //partie movement
        gameState._ecs.runComponentFunction<RType::Components::Action>();  // handle action player
        gameState._ecs.runComponentFunction<RType::Components::Velocity>();  // move entity

        //partie collision
        gameState._ecs.runComponentFunction<RType::Components::Hitbox>();  // handle collision
        gameState._ecs.runComponentFunction<RType::Components::Clickable>();  // check click on the few entity who has this component

        //partie game rule
        gameState._ecs.runComponentFunction<RType::Components::Life>();  // handle life
        gameState._ecs.runComponentFunction<RType::Components::Chrono>();  // handle chrono

        //partie render
        gameState._ecs.runComponentFunction<RType::Components::Sprite>();  // render sprite
        gameState._ecs.runComponentFunction<RType::Components::HealthViewer>();//render health
        gameState._ecs.runComponentFunction<RType::Components::HitboxViewer>();  // render hitbox

        // Check espace input
        if (Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager()
        .receivedInput(Rengine::Graphics::UserInputTypeKeyboardSpecialPressed, {Rengine::Graphics::UserInputKeyboardSpecialESCAPE})) {
            return State::StateMenu;
        }
        if (Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager()
        .receivedInput(Rengine::Graphics::UserInputTypeMouseLeftClick)) {
            auto& player = gameState._ecs.getEntity(gameState._playerEntityId);
            auto& sp = player.getComponent<Components::Sprite>();

            sp.getSprite()->flip();
        }
        return State::StateGame;
    }

    void GameState::initScenes(void)
    {
        // Laod level scene
        std::function<State(GameState&)> funScene1(loadLevelFunction);

        this->_sceneManager.setSceneFunction<State, GameState&>(GameScenes::GameScenesLoadLevel, funScene1);
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
        RType::Components::Sprite& sp = player.addComponent<RType::Components::Sprite>(enConfig.getSprite().getSpecs());

        player.addComponent<RType::Components::Action>(this->_sceneManager, Components::ActionSourceUserInput);
        player.addComponent<RType::Components::Configuration>(enConfig);
        player.addComponent<RType::Components::Position>(0, 0);
        player.addComponent<RType::Components::Buff>();
        player.addComponent<RType::Components::Hitbox>(enConfig.getHitbox());
        player.addComponent<RType::Components::Clickable>( [](void){} );  // damn fork bomb is an empty lambda
        player.addComponent<Components::Relationship>();
        player.addComponent<RType::Components::HitboxViewer>(enConfig.getHitbox().size.x, enConfig.getHitbox().size.y);
        player.addComponent<RType::Components::Metadata>();
        player.addComponent<RType::Components::Life>(enConfig.getStats().hp);
        player.addComponent<RType::Components::HealthViewer>(enConfig.getStats().hp);

        player.setComponentsDestroyFunction(
           [](Rengine::Entity& en) {
                en.removeComponent<RType::Components::Action>();
                en.removeComponent<RType::Components::Configuration>();
                en.removeComponent<RType::Components::Position>();
                en.removeComponent<RType::Components::Sprite>();
                en.removeComponent<RType::Components::Buff>();
                en.removeComponent<RType::Components::Hitbox>();
                en.removeComponent<RType::Components::Clickable>();
                en.removeComponent<RType::Components::Relationship>();
                en.removeComponent<RType::Components::HitboxViewer>();
                en.removeComponent<RType::Components::Metadata>();
                en.removeComponent<RType::Components::HealthViewer>();
                en.removeComponent<RType::Components::Life>();
            }
        );
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
        }
    }

}  // namespace RType
