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
        void setX(float x)
        {
            this->_x = x;
        }
        void setY(float y)
        {
            this->_y = y;
        }
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
            float changeX = config->get().getConfig().getStats().speedX * deltatime;
            float changeY = config->get().getConfig().getStats().speedY * deltatime;
            pos->get().set(
                    {pos->get().getX() + (velocity._x * changeX),
                    pos->get().getY() + (velocity._y * changeY)}
            );
        }

    private:
        float _x = 0;
        float _y = 0;
};
}  // namespace Components
}  // namespace RType

#endif  // SRC_COMPONENTS_VELOCITY_HPP_
