#include "../include/Command/Entity.hpp"

bool handleGlobalEntity(Message<Communication::TypeDetail> &msg, User &user, Server &server) {
    switch (msg.header.type.precision) {
        case Communication::main::EntityActionPrecision::Move: {
            Entity entity = server.getUsers()->at(user);
            std::cout << user.endpoint << " is moving entity " << entity << std::endl;
            float x, y;

            msg >> x >> y;
            server.getMaker()->velo[entity].value().x = x;
            server.getMaker()->velo[entity].value().y = y;
            break;
        }

        default:
            break;
    }
    return true;
}
