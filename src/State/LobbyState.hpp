//
#ifndef SRC_STATE_LOBBYSTATE_HPP_
#define SRC_STATE_LOBBYSTATE_HPP_

#include <rengine/Rengine.hpp>
#include <rengine/RengineNetworkClient.hpp>

#include "AState.hpp"
#include "GameState.hpp"
#include "src/State/State.hpp"
#include "src/Network/EntityAction.hpp"

namespace RType {

    enum LobbyScenes {
        LobbySceneInitConnexion,
        LobbySceneRun
    };

    struct LobbyInfo {
        std::string serverIp = "0.0.0.0";
        uint16_t port = 4242;
        std::string playerJson = "assets/entities/playerDefault.json";
    };

    class LobbyState : public AState {
        public:
            LobbyState(Rengine::ECS& ecs);
            ~LobbyState(void) = default;
            /*
            * @fn registerComponents
            * @brief Registers the neccessary component to the ECS.
            */
            void registerComponents(void);
            friend State initLobbyConnexion(LobbyState &LobbyState);

            friend State runLevel(LobbyState &LobbyState);

            void initScenes(void);

            /*
            * @fn run
            * @return State The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            State run(void);

            /*
            * @fn setLobbyInfo
            * @param lobbyInfo The data of the lobby.
            * @brief Set the lobbyInfo from the menu by the StateManager.
            */
            void setLobbyInfo(const LobbyInfo& lobbyInfo) noexcept;

        private:
            LobbyInfo _lobbyInfo;
            std::unique_ptr<ClientTCP<Network::Communication::TypeDetail>> _client;
    };

}  // namespace RType

#endif  // SRC_STATE_LOBBYSTATE_HPP_
