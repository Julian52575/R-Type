
#include "EntityAction.hpp"

namespace RType {
    namespace Network {

        std::ostream& operator<<(std::ostream& os, const EntityAction& action)
        {
            os << action.type;
            if (action.type == EntityActionTypeMove) {
                os << "Move strength: " << action.data.moveVelocity;
            }
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const EntityActionType& actionType)
        {
            switch (actionType) {
                case EntityActionTypeMove:
                    os << "EntityActionTypeMove";
                    break;

                case EntityActionTypeShoot1:
                    os << "EntityActionTypeShoot1";
                    break;

                case EntityActionTypeShoot2:
                    os << "EntityActionTypeShoot2";
                    break;

                case EntityActionTypeShoot3:
                    os << "EntityActionTypeShoot3";
                    break;

                case EntityActionTypeDodge:
                    os << "EntityActionTypeDodge";
                    break;

                case EntityActionTypeBarrier:
                    os << "EntityActionTypeBarrier";
                    break;

                case EntityActionTypeDeath:
                    os << "EntityActionTypeDeath";
                    break;

                case EntityActionTypeUltimate:
                    os << "EntityActionTypeUltimate";
                    break;

                default:
                    os << "Unrecognized EntityActionType";
                    break;
            }
            return os;
        }

    }  // namespace Network
}  // namespace RType
