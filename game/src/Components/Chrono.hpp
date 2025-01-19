#pragma once

#include <functional>
#include <rengine/Rengine.hpp>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Entity.hpp>

namespace RType {
    namespace Components {
        class Chrono {
            public:
                Chrono(const std::function<void(void)> fun, float delay);
                ~Chrono(void) = default;
                static void componentFunction(Rengine::ECS& ecs, Chrono& thiss, Rengine::Entity& entity);

            private:
                std::function<void(void)> _fun;
                float _delay;
                float _time;
        };

    }
}