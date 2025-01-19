#include "Chrono.hpp"

namespace RType{
namespace Components {
Chrono::Chrono(const std::function<void(void)> fun, float delay)
    : _fun(fun), _delay(delay), _time(0)
{
}

void Chrono::componentFunction(Rengine::ECS& ecs, Chrono& chrono, Rengine::Entity& entity)
{
    chrono._time += Rengine::Clock::getElapsedTime();
    if (chrono._time >= chrono._delay) {
        chrono._fun();
        chrono._time = 0;
    }
}
}  // namespace Components
}  // namespace RType
