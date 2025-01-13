/*
#include "../include/Command/Entity.hpp"

bool handleGlobalEntity(Message<Communication::TypeDetail> &msg, User &user, Server &server) {
    switch (msg.header.type.precision) {
        case Communication::main::EntityActionPrecision::Move: {
            Entity &entity = server.getUsers().at(user);
            float x, y;

            msg >> x >> y;
            server.getMaker()->velo[entity].value().x = x;
            server.getMaker()->velo[entity].value().y = y;
            break;
        }

        case Communication::main::EntityActionPrecision::Shoot1: {
            Entity entity = server.getUsers().at(user);

            if (server.getMaker()->attack[entity].value().canAttack()) {
                Entity &bullet = server.MakeEntity(2);
                float x, y;
                x = server.getMaker()->pos[entity].value().x;
                y = server.getMaker()->pos[entity].value().y;
                server.getMaker()->UpdatePosition(bullet, x, y);
                server.getMaker()->UpdateGroup(bullet, "player");
                Message<Communication::TypeDetail> BroadCastMessage;
                BroadCastMessage.header.type = {Communication::EntityInfo, Communication::main::EntityInfoPrecision::NewEntity};
                BroadCastMessage.header.size = 0;
                uint16_t configurationId = 2;
                uint16_t id = bullet.getId();

                std::cout << "[SERVER] New bullet with id: " << id << " from " << user.endpoint << " at " << x << " " << y << std::endl;
                BroadCastMessage << id << x << y << configurationId;
                server.getServer().SendAll(BroadCastMessage);
                server.getMaker()->attack[entity].value().reset();
            }
            break;
        }

        default:
            break;
    }
    return true;
}
*/
