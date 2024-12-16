#include "../include/Command/Connexion.hpp"

/**
 * @brief Handles the client connection request.
 *
 * This function processes a client connection request, creates a new entity for the client, 
 * assigns it to the user, and sends a response message containing the entity ID. It also broadcasts 
 * the new entity information to all connected clients.
 *
 * @param msg The message containing the connection details.
 * @param user The user requesting the connection.
 * @param server The server instance used to handle the connection.
 * @return true if the connection was successfully handled, false otherwise.
 */
static bool handleClientConnexion(Message<Communication::TypeDetail> &msg, User &user, Server &server) {
    if (server.getUsers().find(user) != server.getUsers().end()) {
        std::cerr << "[SERVER] User already connected: " << user.endpoint << std::endl;
        return false;
    }
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

/**
 * @brief Handles the client disconnection request.
 *
 * This function processes a client disconnection request, removes the associated entity from 
 * the server, and broadcasts a disconnection message to all connected clients.
 *
 * @param msg The message containing the disconnection details.
 * @param user The user requesting the disconnection.
 * @param server The server instance used to handle the disconnection.
 * @return true if the disconnection was successfully handled, false otherwise.
 */
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

/**
 * @brief Handles the request for the playable entity ID.
 *
 * This function processes a request for the playable entity ID for the given user and 
 * sends the ID back to the user.
 *
 * @param msg The message containing the request details.
 * @param user The user making the request.
 * @param server The server instance used to handle the request.
 * @return true if the request was successfully handled, false otherwise.
 */
static bool handleRequestPlayableEntity(Message<Communication::TypeDetail> &msg, User &user, Server &server) {
    Entity &entity = server.getUsers().at(user);

    Message<Communication::TypeDetail> responseMessage;
    responseMessage.header.type = {Communication::ConnexionDetail, Communication::main::ConnexionDetailPrecision::PlayableEntityInGameId};
    uint16_t id = entity.getId();
    responseMessage << id;
    server.getServer().Send(responseMessage, user);
    return true;
}

/**
 * @brief Handles global connection-related actions based on the received message.
 *
 * This function processes the received message to determine the connection-related action 
 * (client connection, client disconnection, or request for playable entity) and forwards 
 * the message to the corresponding handler function.
 *
 * @param msg The message containing the connection-related action details.
 * @param user The user performing the connection-related action.
 * @param server The server instance used to handle the action.
 * @return true if the action was successfully handled, false otherwise.
 */
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
