//
#ifndef SRC_NETWORK_ENTITYACTION_HPP_
#define SRC_NETWORK_ENTITYACTION_HPP_
#include <iostream>
#include <iterator>
#include <rengine/src/Graphics/Vector.hpp>

namespace RType {
namespace Network {
    enum EntityActionType {
        EntityActionTypeMove   = 0x00,
        EntityActionTypeShoot1 = 0x01,
        EntityActionTypeShoot2 = 0x02,
        EntityActionTypeShoot3 = 0x03,
        EntityActionTypeDodge  = 0x04,
        EntityActionTypeBarrier = 0x05,
        EntityActionTypeDeath  = 0x06,
        EntityActionTypeUltimate = 0x07
    };  // enum EntityAction
    union EntityActionData {
        Rengine::Graphics::vector2D<float> moveVelocity;
    };  // union EntityActionData
struct EntityAction {
    EntityActionType type = (EntityActionType) 0xff;
    EntityActionData data = {0};
};  // struct EntityAction
    std::ostream& operator<<(std::ostream& os, const EntityAction& action);
    std::ostream& operator<<(std::ostream& os, const EntityActionType& actionType);
}  // namespace Network
}  // namespace RType
#endif  // SRC_NETWORK_ENTITYACTION_HPP_
