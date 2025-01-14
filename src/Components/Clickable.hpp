// Needs hitbox and position component
#ifndef SRC_COMPONENTS_CLICKABLE_HPP_
#define SRC_COMPONENTS_CLICKABLE_HPP_

#include <functional>
#include <rengine/Rengine.hpp>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Entity.hpp>

namespace RType {
    namespace Components {

        /**
        * @addtogroup RType::Config
        * @namespace RType
        * @class Clickable
        * @brief Execute the function if a click was detected on the entity hitbox
        */
        class Clickable {
            public:
                Clickable(const std::function<void(void)> fun);
                ~Clickable(void) = default;
                static void componentFunction(Rengine::ECS& ecs, Clickable& thiss, Rengine::Entity& entity);

            private:
                std::function<void(void)> _fun;
        };

    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_CLICKABLE_HPP_
