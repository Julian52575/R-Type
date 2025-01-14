//
#ifndef SRC_STATE_LOBBYSTATE_HPP_
#define SRC_STATE_LOBBYSTATE_HPP_

#include <rengine/Rengine.hpp>

#include "AState.hpp"
#include "GameState.hpp"
#include "src/State/State.hpp"


namespace RType {

    enum LobbyScenes {
        LobbyScenesNA,
        LobbyScenesPlaceholder,
        LobbyScenesEliott
    };

    struct LobbyInfo {
        std::string serverIp = "0.0.0.0";
        uint16_t port = 4242;
        std::string playerJson = "assets/entities/playerDefault.json";
    };

    class LobbyState : public AState {
        public:
            LobbyState(Rengine::ECS& ecs) : AState(ecs)
            {
            }
            ~LobbyState(void) = default;
            /*
            * @fn registerComponents
            * @brief Registers the neccessary component to the ECS.
            */
            void registerComponents(void)
            {
                // WIP
            }
            /*
            * @fn run
            * @return State The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            State run(void)
            {
                std::cout << "Running lobby with ip = " << this->_lobbyInfo.serverIp << ". port = " << this->_lobbyInfo.port << ". playerJson = " << this->_lobbyInfo.playerJson << std::endl;
                std::cout << "Exit lobby and running game..." << std::endl;
                return StateGame;
            }
            /*
            * @fn setLobbyInfo
            * @param lobbyInfo The data of the lobby.
            * @brief Set the lobbyInfo from the menu by the StateManager.
            */
            void setLobbyInfo(const LobbyInfo& lobbyInfo) noexcept
            {
                this->_lobbyInfo = lobbyInfo;
            }

        private:
            LobbyInfo _lobbyInfo;
    };

}  // namespace RType

#endif  // SRC_STATE_LOBBYSTATE_HPP_
