//
#ifndef SRC_STATE_MENUSTATE_HPP_
#define SRC_STATE_MENUSTATE_HPP_

#include <rengine/Rengine.hpp>

#include "AState.hpp"
#include "State.hpp"

namespace RType {

    enum RTypeMenuScenes {
        // Placeholder enum
        RTypeMenuScenesNA,
        RTypeMenuScenesAttractMode,
        RTypeMenuScenesSelectLobby,
    };

    class MenuState : public AState {
        public:
            MenuState(Rengine::ECS& ecs) : AState(ecs) {}
            ~MenuState(void) = default;
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
                // WIP
                return StateMenu;
            }
    };

}  // namespace RType

#endif  // SRC_STATE_MENUSTATE_HPP_
