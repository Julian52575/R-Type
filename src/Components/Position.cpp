#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Graphics/Vector.hpp>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <rengine/Rengine.hpp>

#include "../Network/EntityAction.hpp"
#include "Configuration.hpp"
#include "Position.hpp"
#include "src/Config/AttackBuffTypeEnum.hpp"
#include "src/Config/AttackConfig.hpp"
#include "src/Config/EntityConfig.hpp"
#include "src/Config/EntityConfigResolver.hpp"

namespace RType {
namespace Components {
    Position::Position(void){}
    Position::Position(float x, float y) : _position({x, y}) {}
    Position::~Position(void){}
    const Rengine::Graphics::vector2D<float>& Position::getVector2D(void) noexcept
    {
        return this->_position;
    }
    const Rengine::Graphics::vector2D<float>& Position::getVector2D(void) const noexcept
    {
        return this->_position;
    }
    void Position::set(const Rengine::Graphics::vector2D<float>& pos)
    {
        this->_position = pos;
    }
    void Position::setX(float x)
    {
        this->_position.x = x;
    }
    void Position::setY(float y)
    {
        this->_position.y = y;
    }
    float Position::getX(void) const noexcept
    {
        return this->_position.x;
    }
    float Position::getY(void) const noexcept
    {
        return this->_position.y;
    }
}  // namespace Components
}  // namespace RType
