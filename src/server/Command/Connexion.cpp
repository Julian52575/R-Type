/*

#include "../include/Command/Connexion.hpp"

static bool handleClientConnexion(Message<Communication::TypeDetail> &msg, User &user, Server &server) {
    Entity &entity = server.MakeEntity(3);
    uint16_t configurationId = 3;
    server.getUsers().insert({user, entity});

    Message<Communication::TypeDetail> responseMessage;
    responseMessage.header.type = {Communication::ConnexionDetail, Communication::main::ConnexionDetailPrecision::PlayableEntityInGameId};
    uint16_t id = entity.getId();
    responseMessage << id;
    server.getServer().Send(responseMessage, user);
    std::cout << "[SERVER] New client connexion with id: " << id << " from " << user.endpoint << std::endl;

    Message<Communication::TypeDetail> BroadCastMessage;
    BroadCastMessage.header.type = {Communication::EntityInfo, Communication::main::EntityInfoPrecision::NewEntity};
    BroadCastMessage.header.size = 0;
    float x = server.getMaker()->pos[entity].value().x;
    float y = server.getMaker()->pos[entity].value().y;
    BroadCastMessage << id << x << y << configurationId;
    server.getServer().SendAll(BroadCastMessage);
    return true;
}

static bool handleClientDisconnect(Message<Communication::TypeDetail> &msg, User &user, Server &server) {
    Entity &entity = server.getUsers().at(user);
    uint16_t id = entity.getId();

    std::cout << "[SERVER] Client disconnected with id: " << entity.getId() << " from " << user.endpoint << std::endl;
    server.destroy_entity(entity);
    server.getUsers().erase(user);
    server.getServer().RemoveUser(user);
    Message<Communication::TypeDetail> BroadCastMessage;

    BroadCastMessage.header.type = {Communication::ConnexionDetail, Communication::main::ConnexionDetailPrecision::ServerDisconnect};
    BroadCastMessage.header.size = 0;
    BroadCastMessage << id;

    server.getServer().SendAll(BroadCastMessage);
    return true;
}

static bool handleRequestPlayableEntity(Message<Communication::TypeDetail> &msg, User &user, Server &server) {
    Entity &entity = server.getUsers().at(user);

    Message<Communication::TypeDetail> responseMessage;
    responseMessage.header.type = {Communication::ConnexionDetail, Communication::main::ConnexionDetailPrecision::PlayableEntityInGameId};
    uint16_t id = entity.getId();
    responseMessage << id;
    server.getServer().Send(responseMessage, user);
    return true;
}


bool handleGlobalConnexion(Message<Communication::TypeDetail> &msg, User &user, Server &server) {
    switch (msg.header.type.precision) {
        case Communication::main::ConnexionDetailPrecision::ClientConnexion:
            return handleClientConnexion(msg, user, server);

        case Communication::main::ConnexionDetailPrecision::ClientDisconnect:
            return handleClientDisconnect(msg, user, server);

        case Communication::main::ConnexionDetailPrecision::RequestPlaybleEntity:
            return handleRequestPlayableEntity(msg, user, server);

        default:
            std::cerr << "[SERVER] Unknown ConnexionDetailPrecision: " << msg.header.type.precision << std::endl;
            return false;
    }
}
*/
