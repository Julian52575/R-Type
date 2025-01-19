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
#include "src/Config/EntityConfig.hpp"
#include "src/Game/EntityMaker.hpp"
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
#include "src/Game/Team.hpp"
#include "src/Config/ConfigurationIdResolver.hpp"
#include "NetworkStructs.hpp"

namespace RType {

    GameState::GameState(Rengine::ECS& ecs, AccessibilitySettings& access, NetworkInfo& networkInfo, LobbyInfo& lobbyInfo)
        : AState(ecs), _lobbyInfo(lobbyInfo),
        _levelManager(ecs, this->_sceneManager),
        _accessibilitySettings(access),
        _networkInfo(networkInfo)
    {
        this->_clientTCP = nullptr;
        this->_clientUDP = nullptr;
        this->initScenes();
        // this->_sceneManager.setScene(GameScenes::GameScenesLoadLevel);
        this->_sceneManager.setScene(GameScenes::GameScenesInitNetwork);
        this->_playerEntityId = RTYPE_NO_PLAYER_ENTITY_ID;
    }

    std::unique_ptr<ClientTCP<Network::Communication::TypeDetail>> &GameState::getClientTCP(void) noexcept
    {
        return this->_clientTCP;
    }

    std::unique_ptr<ClientUDP<Network::Communication::TypeDetail>> &GameState::getClientUDP(void) noexcept
    {
        return this->_clientUDP;
    }


    void componentActionServerFunction(Rengine::ECS& ecs, Components::Action& actionComponent, Rengine::Entity& entity, GameState& gameState)
    {
        updateDeltatimes(actionComponent);
        std::optional<std::reference_wrapper<RType::Components::Configuration>> entityConfig = entity.getComponentNoExcept<RType::Components::Configuration>();
        std::optional<std::reference_wrapper<RType::Components::Position>> pos = entity.getComponentNoExcept<RType::Components::Position>();
        if (entityConfig.has_value() == false || pos.has_value() == false) {
            return;
        }

        if (actionComponent.getActionSource() != Components::ActionSource::ActionSourceUserInput) {
            return;
        }
        actionComponent.updateFromSource();
        const Rengine::Graphics::vector2D<float>& currentPos = pos->get().getVector2D();
        Rengine::Graphics::vector2D<float> newPos = currentPos;
        for (auto it : actionComponent) {
            // Move
            if (it.type == Network::EntityActionTypeMove) {
                actionComponent.handleMove(it, entityConfig.value(), pos.value());
                Message<RType::Network::Communication::TypeDetail> msg;
                msg.header.type = {RType::Network::Communication::Type::EntityAction, RType::Network::Communication::main::EntityActionPrecision::EntityActionTypeMove};
                msg.header.size = 0;
                msg << it.data.moveVelocity.x << it.data.moveVelocity.y;
                gameState.getClientUDP()->Send(msg);
            }
            // Shoot1 -> 3
            if (Network::EntityActionTypeShoot1 <= it.type && it.type <= Network::EntityActionTypeShoot3) {
                switch (it.type) {
                    case Network::EntityActionTypeShoot1: {
                        Message<RType::Network::Communication::TypeDetail> msg;
                        msg.header.type = {RType::Network::Communication::Type::EntityAction, RType::Network::Communication::main::EntityActionPrecision::EntityActionTypeShoot1};
                        msg.header.size = 0;
                        gameState.getClientUDP()->Send(msg);
                        break;
                    }

                    case Network::EntityActionTypeShoot2: {
                        Message<RType::Network::Communication::TypeDetail> msg;
                        msg.header.type = {RType::Network::Communication::Type::EntityAction, RType::Network::Communication::main::EntityActionPrecision::EntityActionTypeShoot2};
                        msg.header.size = 0;
                        gameState.getClientUDP()->Send(msg);
                        break;
                    }

                    case Network::EntityActionTypeShoot3: {
                        Message<RType::Network::Communication::TypeDetail> msg;
                        msg.header.type = {RType::Network::Communication::Type::EntityAction, RType::Network::Communication::main::EntityActionPrecision::EntityActionTypeShoot3};
                        msg.header.size = 0;
                        gameState.getClientUDP()->Send(msg);
                        break;
                    }

                    default:
                        break;
                }
            }
        }  // for it
        actionComponent.clear();
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
        std::function<void(Rengine::ECS&, RType::Components::Action&, Rengine::Entity&, GameState&)> actionFunction = componentActionServerFunction;
        this->_ecs.setComponentFunction<RType::Components::Action, GameState&>(actionFunction);
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
        std::cout << "Playing " << jsonPath << std::endl;
        this->_levelManager.loadLevel(jsonPath);
        uint16_t configID = RType::Config::EntityConfigurationIdResolverSingletone::get().get(this->_lobbyInfo.playerJson);
        Message<Network::Communication::TypeDetail> msg;
        msg.header.type = {Network::Communication::Type::ConnexionDetail, Network::Communication::main::ConnexionDetailPrecision::ClientConnexion};
        msg.header.size = 0;
        msg << configID << this->_clientUDP->getLocalEndpoint();
        this->_clientTCP->Send(msg);
    }

    State GameState::run(void)
    {
        State s = this->_sceneManager.callCurrentSceneFunction<State, GameState&>(*this);

        return s;
    }

    Rengine::Entity &GameState::getOrCreateEntity(Rengine::Entity::size_type id, uint16_t configurationID) {
        for (auto &entry : this->_entities) {
            if (entry.first == id) {
                if (this->_ecs.isEntityActive(entry.second))
                    return this->_ecs.getEntity(entry.second);
                else
                    continue;
            }
        }
        Rengine::Entity &entity = EntityMaker::make(this->_ecs, configurationID);
        RType::Config::EntityConfig enConfig = entity.getComponent<RType::Components::Configuration>().getConfig();
        RType::Components::Sprite& sp = entity.addComponent<RType::Components::Sprite>(enConfig.getSprite().getSpecs());

        entity.addComponent<RType::Components::HitboxViewer>(enConfig.getHitbox().size.x, enConfig.getHitbox().size.y);
        entity.addComponent<RType::Components::HealthViewer>(enConfig.getStats().hp);

        entity.setComponentsDestroyFunction(
           [](Rengine::Entity& en) {
                en.removeComponent<RType::Components::HitboxViewer>();
                en.removeComponent<RType::Components::HealthViewer>();
            }
        );
        this->_entities.push_back({id, Rengine::Entity::size_type(entity)});
        return entity;
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

        // Loading a random level
        for (auto file : std::filesystem::directory_iterator("assets/levels/")) {
            configVector.push_back(file.path().string());
        }
        if (configVector.size() == 0) {
            gameState._levelManager.completeClear();
            return State::StateMenu;
        }
        idx = Rengine::RNG::rngFunction() % configVector.size();
        gameState.loadLevel(configVector[idx]);
        gameState._sceneManager.setScene(GameScenes::GameScenesPlay);
        return State::StateGame;
    }

    State playFunction(GameState& gameState)
    {
        Rengine::SparseArray<RType::Components::Hitbox>& hitboxs = gameState._ecs.getComponents<RType::Components::Hitbox>();
        Rengine::SparseArray<RType::Components::Position>& positions = gameState._ecs.getComponents<RType::Components::Position>();
        Rengine::SparseArray<RType::Components::Relationship>& relationships = gameState._ecs.getComponents<RType::Components::Relationship>();
        Rengine::SparseArray<RType::Components::Life>& lifes = gameState._ecs.getComponents<RType::Components::Life>();

        for (std::optional<Message<RType::Network::Communication::TypeDetail>> msg = gameState._clientTCP->Receive(); msg; msg = gameState._clientTCP->Receive()) {
            if (msg->header.type.type == RType::Network::Communication::Type::ConnexionDetail && msg->header.type.precision == RType::Network::Communication::main::ConnexionDetailPrecision::PlayableEntityInGameId) {
                Rengine::Entity::size_type id;
                *msg >> id;
                uint16_t configID = RType::Config::EntityConfigurationIdResolverSingletone::get().get(gameState._lobbyInfo.playerJson);
                Rengine::Entity &entity = gameState.getOrCreateEntity(id, configID);

                entity.addComponent<RType::Components::Action>(RType::Components::ActionSourceUserInput);
                entity.addComponent<RType::Components::Clickable>( [](void){} );

                entity.setComponentsDestroyFunction(
                   [](Rengine::Entity& en) {
                        en.removeComponent<RType::Components::Action>();
                        en.removeComponent<RType::Components::Clickable>();
                    }
                );
                gameState._playerEntityId = Rengine::Entity::size_type(entity);
            }

            if (msg->header.type.type == RType::Network::Communication::Type::EntityInfo && msg->header.type.precision == RType::Network::Communication::main::EntityInfoPrecision::DeleteEntity) {
                uint64_t id;
                *msg >> id;
                try {
                    gameState._entities.erase(std::remove_if(gameState._entities.begin(), gameState._entities.end(), [id](const std::pair<Rengine::Entity::size_type, Rengine::Entity::size_type>& entry) {
                        return entry.first == id;
                    }), gameState._entities.end());
                    gameState._ecs.removeEntity(id);
                } catch (std::exception& e) {
                    std::cerr << "Error on delete entity: " << e.what() << std::endl;
                }
            }
        }

        for (std::optional<Message<RType::Network::Communication::TypeDetail>> msg = gameState._clientUDP->Receive(); msg; msg = gameState._clientUDP->Receive()) {
            if (msg->header.type.type == RType::Network::Communication::Type::EntityInfo && msg->header.type.precision == RType::Network::Communication::main::EntityInfoPrecision::InfoAll) {
                uint16_t health;
                uint16_t maxHealth;
                uint64_t id;
                uint16_t configID;
                float posX;
                float posY;
                *msg >> configID >> posY >> posX >> maxHealth >> health >> id;
                Rengine::Entity& entity = gameState.getOrCreateEntity(id, configID);
                if (positions[entity].has_value()) {
                    positions[entity]->set({posX, posY});
                }
                if (lifes[entity].has_value()) {
                    lifes[entity]->setHp(health);
                    lifes[entity]->setMaxHp(maxHealth);
                }
            }
        }

        //partie movement
        gameState._ecs.runComponentFunction<RType::Components::Action>(gameState);  // update action
        gameState._ecs.runComponentFunction<RType::Components::Velocity>();  // move entity

        gameState._ecs.runComponentFunction<RType::Components::Clickable>();  // check click on the few entity who has this component

        //partie render
        gameState._ecs.runComponentFunction<RType::Components::Sprite>();  // render sprite
        gameState._ecs.runComponentFunction<RType::Components::HealthViewer>();//render health
        gameState._ecs.runComponentFunction<RType::Components::HitboxViewer>();  // render hitbox

        // Check espace input
        if (Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager()
        .receivedInput(Rengine::Graphics::UserInputTypeKeyboardSpecialPressed, {Rengine::Graphics::UserInputKeyboardSpecialESCAPE})) {
            gameState._sceneManager.setScene(GameScenes::GameScenesLoadLevel);
            gameState._levelManager.completeClear();
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

    State InitNetwork(GameState& gameState) {
        try {
            gameState._clientTCP = std::make_unique<ClientTCP<Network::Communication::TypeDetail>>(gameState._networkInfo.ip, gameState._networkInfo.TCPPort);
            gameState._clientUDP = std::make_unique<ClientUDP<Network::Communication::TypeDetail>>(gameState._networkInfo.ip, gameState._networkInfo.UDPPort);
        } catch (const std::exception& e) {
            std::cerr << "Error" << e.what() << std::endl;
            gameState._levelManager.completeClear();
            return State::StateLobby;
        }
        gameState._sceneManager.setScene(GameScenes::GameScenesLoadLevel);
        return State::StateGame;
    }

    void GameState::initScenes(void)
    {
        std::function<State(GameState&)> funScene0(InitNetwork);
        this->_sceneManager.setSceneFunction<State, GameState&>(GameScenes::GameScenesInitNetwork, funScene0);

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
        RType::Config::EntityConfig enConfig;
        Rengine::Entity& player = RType::EntityMaker::make(this->_ecs, jsonPath, Team::TeamPlayer, &enConfig);
        RType::Components::Sprite& sp = player.addComponent<RType::Components::Sprite>(enConfig.getSprite().getSpecs());

        player.addComponent<RType::Components::Action>(RType::Components::ActionSourceUserInput);
    #ifdef DEBUG
        player.addComponent<RType::Components::HitboxViewer>(enConfig.getHitbox().size.x, enConfig.getHitbox().size.y);
        player.addComponent<RType::Components::HealthViewer>(enConfig.getStats().hp);
    #endif

        player.setComponentsDestroyFunction(
           [](Rengine::Entity& en) {
                en.removeComponent<RType::Components::Action>();
                en.removeComponent<RType::Components::Clickable>();
            #ifdef DEBUG
                en.removeComponent<RType::Components::HitboxViewer>();
                en.removeComponent<RType::Components::HealthViewer>();
            #endif
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
            if (spRelationship[index]->isRelated(uint64_t(this->_playerEntityId))) {
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
            // #warning Debug print
        }
    }

}  // namespace RType
