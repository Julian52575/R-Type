//
#ifndef SRC_COMPONENTS_POSITION_HPP_
#define SRC_COMPONENTS_POSITION_HPP_
#include <cstddef>
#include <cstdint>
#include <rengine/Rengine.hpp>
#include <rengine/src/Graphics/Vector.hpp>

namespace RType {

    namespace Components {

        class Position {
            public:
                Position(void);
                Position(float x, float y);
                ~Position(void);
                const Rengine::Graphics::vector2D<float>& getVector2D(void) const noexcept;
                const Rengine::Graphics::vector2D<float>& getVector2D(void) noexcept;
                void set(const Rengine::Graphics::vector2D<float>& pos);
                void setX(float x);
                void setY(float y);
                static void componentFunction(Rengine::ECS& ecs, RType::Components::Position& pos, Rengine::Entity& entity);

            private:
                Rengine::Graphics::vector2D<float> _position = {0, 0};
        };
    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_POSITION_HPP_
