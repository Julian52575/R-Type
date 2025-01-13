/*
#include "../include/Command/SendToClient.hpp"

bool sendEntityInfo(Entity &entity, Server &server) {
    Message<Communication::TypeDetail> msg;

    msg.header.type = {Communication::EntityInfo, Communication::main::EntityInfoPrecision::InfoAll};
    msg.header.size = 0;

    uint16_t id = entity.getId();
    uint16_t configurationId = entity.getConfigId();
    float x = server.getMaker()->pos[entity].value().x;
    float y = server.getMaker()->pos[entity].value().y;
    msg << id << x << y << configurationId;
    server.getServer().SendAll(msg);
    return true;
}
*/
