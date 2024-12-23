/*
*/

#ifndef SRC_ECS_HPP_
#define SRC_ECS_HPP_
#include "ComponentRegistry.hpp"
#include "Entity.hpp"

namespace Rengine {
    /**
     * @addtogroup Rengine
     * @namespace Rengine
     * @class ECS
     * @brief The main class of the game engine. Everything ECS related passes by this class.
    */
    class ECS {
        public:
            /**
            * @fn ECS
            * @brief Create a new instance of ECS.
            */
            ECS(void);
            /**
            * @fn ~ECS
            * @brief Destroy an instance of ECS.
            */
            ~ECS(void) = default;
            /**
            * @fn addEntity
            * @brief Add a blank entity to the ECS.
            */
            Entity& addEntity(void);
            /**
            * @fn removeEntity
            * @param Entity The entity to remove. Note that using that entity again WILL result in a glitch. :(
            * @brief Remove an entity from the ECS.
            */
            void removeEntity(Entity&);



        private:
            ComponentRegistry _registry;
    };  // class ECS
}  // namespace Rengine
#endif  // SRC_ECS_HPP_
