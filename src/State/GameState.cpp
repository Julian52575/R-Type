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


#include "State.hpp"
#include "GameState.hpp"
#include "AState.hpp"


#include "src/Config/LevelConfigResolver.hpp"
#include "src/Components/Buff.hpp"
#include "src/Components/Sprite.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Action.hpp"
#include "src/Components/Components.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Relationship.hpp"

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
        this->_ecs.registerComponent<RType::Components::Hitbox>();
        this->_ecs.registerComponent<RType::Components::Relationship>();

        // Function
        this->_ecs.setComponentFunction<RType::Components::Sprite>(RType::Components::Sprite::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Action>(RType::Components::Action::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Position>(RType::Components::Position::componentFunction);
        this->_ecs.setComponentFunction<RType::Components::Hitbox>(RType::Components::Hitbox::componentFunction);
    }

    void GameState::setNextLevelToLoad(const std::string& level)
    {
        this->_levelToLoad = level;
    }

    void GameState::loadLevel(const std::string& jsonPath)
    {
        // RType::Config::LevelConfigResolver& resolver = RType::Config::LevelConfigResolverSingletone::get();
        // RType::Config::LevelConfig& config = resolver.get(jsonPath);

        // this->loadLevel(config);
        this->_levelManager.loadLevel(jsonPath);

        this->createBackground("assets/entities/Background.json");
        this->createPlayer("assets/entities/skeletonDragon.json");

        std::vector<RType::Config::SceneEntityConfig> enemies = this->_levelManager.getCurrentSceneEnemies();
        for (int i = 0; i < enemies.size(); i++) {
            this->createEnemy(enemies[i].path, {enemies[i].xSpawn, enemies[i].ySpawn});
        }
        
    }

    void GameState::loadLevel(const RType::Config::LevelConfig& levelConfig)
    {
        // #warning Parse level
        // std::vector<RType::Config::SceneConfig> scenes = levelConfig.getScenes();
        
        // this->_currentSceneConfig = scenes[this->_currentSceneIndex];

        // this->createBackground("assets/entities/Background.json");
        // this->createPlayer("assets/entities/skeletonDragon.json");

        // for (int i = 0; i < _currentSceneConfig.enemies.size(); i++) {
        //     this->createEnemy(_currentSceneConfig.enemies[i].path, {_currentSceneConfig.enemies[i].xSpawn, _currentSceneConfig.enemies[i].ySpawn});
        // }
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
        gameState._levelManager.update(Rengine::Graphics::GraphicManagerSingletone::get().getWindow().get()->getDeltaTimeSeconds());
        if (gameState._levelManager.SceneEndCondition()){
            if (!gameState._levelManager.nextScene()){
                gameState._sceneManager.setScene(GameScenes::GameScenesLoadLevel);
                return State::StateGame;
            }

            //detruire les entités courantes
            std::vector<RType::Config::SceneEntityConfig> enemies = gameState._levelManager.getCurrentSceneEnemies();
            for (int i = 0; i < enemies.size(); i++) {
                gameState.createEnemy(enemies[i].path, {enemies[i].xSpawn, enemies[i].ySpawn});
            }

        }

        gameState._ecs.runComponentFunction<RType::Components::Position>();  // move entity
        gameState._ecs.runComponentFunction<RType::Components::Action>();  // handle action player
        gameState._ecs.runComponentFunction<RType::Components::Hitbox>();  // handle collision
        gameState._ecs.runComponentFunction<RType::Components::Sprite>();  // render sprite
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

        player.addComponent<Components::Action>(this->_sceneManager, Components::ActionSourceUserInput);
        player.addComponent<Components::Configuration>(enConfig);
        player.addComponent<Components::Position>(0, 0);
        player.addComponent<Components::Sprite>(enConfig.getSprite().getSpecs());
        player.addComponent<Components::Buff>();
        player.addComponent<Components::Hitbox>(enConfig.getHitbox());
        Components::Relationship& relationship = player.addComponent<Components::Relationship>();

        this->_playerEntityId = Rengine::Entity::size_type(player);
    }

    void GameState::createBackground(const std::string& jsonPath)
    {
        Rengine::Entity& background = this->_ecs.addEntity();
        Config::EntityConfig enConfig(jsonPath);

        background.addComponent<Components::Position>(0, 0);
        background.addComponent<Components::Sprite>(enConfig.getSprite().getSpecs());
    }

    void GameState::createEnemy(const std::string& jsonPath, const Rengine::Graphics::vector2D<int>& pos)
    {
        Rengine::Entity& enemy = this->_ecs.addEntity();
        Config::EntityConfig enConfig(jsonPath);

        enemy.addComponent<Components::Position>(pos.x, pos.y);
        enemy.addComponent<Components::Sprite>(enConfig.getSprite().getSpecs());
        enemy.addComponent<Components::Hitbox>(enConfig.getHitbox());
        enemy.addComponent<Components::Configuration>(enConfig);

        Components::Relationship& relationship = enemy.addComponent<Components::Relationship>();

    }

}  // namespace RType
