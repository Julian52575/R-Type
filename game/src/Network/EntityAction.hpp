//
#ifndef SRC_NETWORK_ENTITYACTION_HPP_
#define SRC_NETWORK_ENTITYACTION_HPP_
#include <iostream>
#include <iterator>
#include <rengine/src/Graphics/Vector.hpp>

namespace RType {
    namespace Network {
        struct Communication {
            enum Type : uint16_t {
                NOOP = 0,
                ConnexionDetail = 1,
                JsonDetail = 2,
                EntityAction = 3,
                EntityInfo = 4,
                LobbyInfo = 5
            };

            union main {
                enum NOOPPrecision : uint16_t {
                    NOOP = 0
                };

                enum ConnexionDetailPrecision : uint16_t {
                    ClientConnexion = 0,
                    ClientDisconnect = 1,
                    ServerAccept = 2,
                    ServerDisconnect = 3,
                    ClientReady = 4,
                    PlayableEntityInGameId = 5,
                    ServerStop = 6,
                    RequestPlaybleEntity = 7,
                    LevelEnd = 8,
                    SceneFinish = 9
                };

                enum EntityActionPrecision : uint16_t {
                    EntityActionTypeMove   = 0x00,
                    EntityActionTypeShoot1 = 0x01,
                    EntityActionTypeShoot2 = 0x02,
                    EntityActionTypeShoot3 = 0x03,
                    EntityActionTypeDodge  = 0x04,
                    EntityActionTypeBarrier = 0x05,
                    EntityActionTypeDeath  = 0x06,
                    EntityActionTypeUltimate = 0x07
                };

                enum EntityInfoPrecision : uint16_t {
                    InfoAll = 0,
                    InGameAndConfigurationId = 1,
                    InfoPosition = 2,
                    InfoHP = 3,
                    InfoMaxHp = 4,
                    InfoSpeed = 5,
                    InfoMaxSpeed = 6,
                    InfoDirection = 7,
                    NewEntity = 8,
                    DeleteEntity = 9
                };

                enum LobbyInfoPrecision : uint16_t {
                    GameCreated = 0,
                    GameDeleted = 1,
                    GameUpdated = 2,
                    CreateGame = 3,
                    RequestedGame = 4,
                    GameInfo = 5,
                    RequestGamesInfo = 6,
                    JoinGame = 7
                };
            };

            struct TypeDetail {
                Type type;
                uint16_t precision;
            };
        };

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
