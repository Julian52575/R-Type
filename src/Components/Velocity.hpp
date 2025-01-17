//
#ifndef SRC_COMPONENTS_VELOCITY_HPP_
#define SRC_COMPONENTS_VELOCITY_HPP_
#include <cstddef>
#include <cstdint>
#include <rengine/Rengine.hpp>

#include "Configuration.hpp"
#include "src/Components/Position.hpp"
namespace RType {
    namespace Components {

        class Velocity {
            public:
                Velocity(void) = default;
                Velocity(float x, float y)
                    : _x(x), _y(y)
                {
                }
                ~Velocity(void) = default;
                static void componentFunction(Rengine::ECS& ecs, RType::Components::Velocity& velocity, Rengine::Entity& entity)
                {
                    std::optional<std::reference_wrapper<Configuration>> config = entity.getComponentNoExcept<Configuration>();
                    std::optional<std::reference_wrapper<Position>> pos = entity.getComponentNoExcept<Position>();

                    if (config.has_value() == false) {
                        return;
                    }
                    if (pos.has_value() == false) {
                        return;
                    }
                    float deltatime = Rengine::Clock::getElapsedTime();
                    float newX = pos->get().getX() + velocity._x + config->get().getConfig().getStats().speedX * deltatime;
                    float newY = pos->get().getY() + velocity._y + config->get().getConfig().getStats().speedY * deltatime;

                    pos->get().set({newX, newY});
                }

            private:
                float _x = 0;
                float _y = 0;
        };
    }  // namespace Components
}  // namespace RType

#endif  // SRC_COMPONENTS_VELOCITY_HPP_
