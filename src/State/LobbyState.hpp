//
#ifndef SRC_STATE_LOBBYSTATE_HPP_

#include <rengine/Rengine.hpp>

#include "AState.hpp"
#include "GameState.hpp"
#include "src/State/State.hpp"


namespace RType {

    enum LobbyScenes {
        LobbyScenesNA,
        LobbyScenesPlaceholder
    };

    class LobbyState : public AState {
        public:
            LobbyState(Rengine::ECS& ecs) : AState(ecs) {}
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
                return StateLobby;
            }

        private:
            std::string _serverIp;
            uint16_t _port;
    };

}  // namespace RType

#endif  // SRC_STATE_LOBBYSTATE_HPP_
