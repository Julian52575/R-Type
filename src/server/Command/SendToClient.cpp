/*
#include "../include/Command/SendToClient.hpp"

/**
 * @brief Sends the information of an entity to all connected clients.
 *
 * This function constructs a message containing the entity's ID, configuration ID, 
 * and position (x, y), and sends it to all connected clients via the server.
 *
 * @param entity The entity whose information is to be sent.
 * @param server The server instance used to send the message.
 * @return true if the entity information was successfully sent, false otherwise.
 */
bool sendEntityInfo(Entity &entity, Server &server) {
    Message<Communication::TypeDetail> msg;

    msg.header.type = {Communication::EntityInfo, Communication::main::EntityInfoPrecision::InfoAll};
    msg.header.size = 0;

    uint16_t id = entity.getId();
    uint16_t configurationId = entity.getConfigId();
    if (server.getMaker()->pos[entity].has_value() == false)
        return false;

    float x = server.getMaker()->pos[entity].value().x;
    float y = server.getMaker()->pos[entity].value().y;
    msg << id << x << y << configurationId;
    server.getServer().SendAll(msg);
    return true;
}
*/
