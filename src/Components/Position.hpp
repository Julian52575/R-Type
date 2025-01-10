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
                Position(void) = default;
                Position(float x, float y) : _position({x, y}) {}
                ~Position(void) = default;
                const Rengine::Graphics::vector2D<float>& getVector2D(void) noexcept
                {
                    return this->_position;
                }
                void set(const Rengine::Graphics::vector2D<float>& pos)
                {
                    this->_position = pos;
                }
                void setX(float x)
                {
                    this->_position.x = x;
                }
                void setY(float y)
                {
                    this->_position.y = y;
                }

            private:
                Rengine::Graphics::vector2D<float> _position = {0, 0};
        };
    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_POSITION_HPP_
