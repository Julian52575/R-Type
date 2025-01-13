// Abstract class for the different program state
// see State.hpp
#ifndef SRC_STATE_ASTATE_HPP_
#define SRC_STATE_ASTATE_HPP_

#include <rengine/Rengine.hpp>

#include "src/Game/SceneManager.hpp"
#include "State.hpp"


namespace RType {

    /**
    * @addtogroup RType
    * @namespace RType
    * @class AState
    * @brief An abstract class for the different program state to follow.
    */
    class AState {
        public:
            AState(Rengine::ECS& ecs) : _ecs(ecs) {}
            ~AState(void) = default;
            /*
            * @fn registerComponents
            * @brief Registers the neccessary component to the ECS.
            */
            virtual void registerComponents(void) = 0;
            /*
            * @fn run
            * @return GameState The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            virtual State run(void) = 0;
            /*
            * @fn getSceneManager
            * @return sceneManager_type The scene manager.
            * @brief Retrives the program's state scene manager.
            */
            SceneManager& getSceneManager(void) noexcept
            {
                return this->_sceneManager;
            }

        protected:
            Rengine::ECS& _ecs;
            SceneManager _sceneManager;
    };
}  // namespace RType
#endif  // SRC_STATE_ASTATE_HPP_
