//
#ifndef SRC_STATE_LOBBYSTATE_HPP_
#define SRC_STATE_LOBBYSTATE_HPP_

#include <rengine/Rengine.hpp>
#include <rengine/RengineNetworkClient.hpp>

#include "AState.hpp"
#include "src/State/State.hpp"
#include "src/Network/EntityAction.hpp"
#include "GameState.hpp"
#include "NetworkStructs.hpp"

#include <ctime>

namespace RType {

    enum LobbyScenes {
        LobbySceneInitConnexion,
        LobbyScenesJoinLobby,
        LobbyScenesCreateLobby,
        LobbySceneRun
    };

    class LobbyState : public AState {
        public:
            LobbyState(Rengine::ECS& ecs, LobbyInfo& lobbyInfo, NetworkInfo& networkInfo);
            ~LobbyState(void) = default;
            /*
            * @fn registerComponents
            * @brief Registers the neccessary component to the ECS.
            */
            void registerComponents(void);
            friend State initLobbyConnexion(LobbyState &lobbyState);
            friend State joinLobbyScene(LobbyState& lobbyState);
            friend State createLobbyScene(LobbyState& lobbyState);
            friend State runLevel(LobbyState &LobbyState);

            void initScenes(void);

            /*
            * @fn run
            * @return State The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            State run(void);

            void handleInput(void);
            void makeGameInfos(std::string name, uint16_t playerCount, time_t time, Rengine::UUID::uuid_t &id);
            void setGameInfos(void);
            DisplayGameInfo& getGameInfoByUuid(Rengine::UUID::uuid_t &id);
            void updateGameInfos(void);

            float _time = 0;
            size_t _begin = 0;
            std::vector<DisplayGameInfo> _displayGameInfos;
            std::shared_ptr<Rengine::Graphics::ASprite> _cursor;

        private:
            LobbyInfo& _lobbyInfo;
            NetworkInfo& _networkInfo;
            std::unique_ptr<ClientTCP<Network::Communication::TypeDetail>> _client;
    };

}  // namespace RType

#endif  // SRC_STATE_LOBBYSTATE_HPP_
