#include "../include/Command/Entity.hpp"

/**
 * @brief Handles entity actions based on the received message.
 *
 * This function processes different types of entity actions, such as movement and shooting, 
 * depending on the precision value in the received message. It updates the state of the 
 * entity accordingly, sending messages to all clients if necessary.
 *
 * @param msg The message containing the entity action details.
 * @param user The user who triggered the entity action.
 * @param server The server instance used to process the action and update entities.
 * @return true if the entity action was successfully handled, false otherwise.
 */
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
