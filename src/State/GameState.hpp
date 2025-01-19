//
#ifndef SRC_STATE_GAMESTATE_HPP_
#define SRC_STATE_GAMESTATE_HPP_
#include <functional>
#include <memory>
#include <optional>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/RengineNetworkClient.hpp>

#include "src/Components/Action.hpp"
#include "src/Game/AccessibilitySettings.hpp"
#include "src/Network/EntityAction.hpp"
#include "src/Config/LevelConfig.hpp"
#include "src/Config/LevelConfigResolver.hpp"
#include "AState.hpp"
#include "src/State/State.hpp"
#include "src/Game/LevelManager.hpp"
#include "NetworkStructs.hpp"

namespace RType {

    /**
    * @addtogroup RType
    * @namespace RType
    * @class GameScenes
    * @brief The different scenes for the game state.
    */
    enum GameScenes {
        GameScenesNA,
        GameScenesInitNetwork,
        GameScenesLoadLevel,
        GameScenesPlay,
        GameScenesPause,
        GameScenesGameOver
    };

    /**
    * @addtogroup RType
    * @namespace RType
    * @class GameState
    * @brief This class is used for the game part of the program.
    */
    class GameState : public AState {
        public:
            GameState(Rengine::ECS& ecs, AccessibilitySettings& access, NetworkInfo& networkInfo, LobbyInfo& lobbyInfo);
            ~GameState(void) = default;
            /**
            * @fn registerComponents
            * @brief Registers the neccessary component to the ECS.
            */
            void registerComponents(void);
            /**
            * @fn run
            * @return State The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            State run(void);
            std::unique_ptr<ClientTCP<Network::Communication::TypeDetail>> &getClientTCP(void) noexcept;
            std::unique_ptr<ClientUDP<Network::Communication::TypeDetail>> &getClientUDP(void) noexcept;

        public:
            /**
            * @fn loadLevel
            * @param std::string The path to a level json
            * @brief Load a level.
            */
            void loadLevel(const std::string& jsonPath);

        public:
            /**
            * @fn loadLevelFunction
            * @param GameState A reference to this GameState class.
            * @brief Load a level and change the scene to play
            */
            friend State loadLevelFunction(GameState& gameState);
            /**
            * @fn playFunction
            * @param GameState A reference to this GameState class.
            * @brief Play the game from the currently loaded level.
            */
            friend State playFunction(GameState& gameState);
            friend State InitNetwork(GameState& gameState);

        /*      Player management       */
        private:
            NetworkInfo& _networkInfo;
            LobbyInfo& _lobbyInfo;
            /**
            * @fn createPlayer
            * @param std::string A path to an entity config
            * @brief Creates the player entity from the config.
            */
            void createPlayer(const std::string& jsonPath);
            std::unique_ptr<ClientTCP<Network::Communication::TypeDetail>> _clientTCP;
            std::unique_ptr<ClientUDP<Network::Communication::TypeDetail>> _clientUDP;
            std::vector<std::pair<Rengine::Entity::size_type, Rengine::Entity::size_type>> _entities;

            Rengine::Entity &getOrCreateEntity(Rengine::ECS::size_type entityId, uint16_t configurationId);

            /*
            * @fn deletePlayer
            * @brief Deletes the player entity.
            */
            void deletePlayer(void);

            void alertPlayer(void);

        protected:
            #define RTYPE_NO_PLAYER_ENTITY_ID (Rengine::ECS::size_type) -1
            Rengine::ECS::size_type _playerEntityId = RTYPE_NO_PLAYER_ENTITY_ID;
            LevelManager _levelManager;
            AccessibilitySettings& _accessibilitySettings;

        // Graphics
        protected:
            Rengine::Graphics::GraphicManagerSingletone _graphicManagerSingletone;

        // Helper
        private:
            void initScenes(void);

    };
}  // namespace RType
#endif  // SRC_STATE_GAMESTATE_HPP_
