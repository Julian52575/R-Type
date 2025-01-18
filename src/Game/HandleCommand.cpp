#include "./game.hpp"
#include "src/Game/EntityMaker.hpp"

void RType::Games::_handleConnexionTCPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg) {
    asio::ip::udp::endpoint UDPClient;
    uint16_t configID;
    msg >> UDPClient >> configID;

    UDPClient.address(asio::ip::make_address(client->getSocket().remote_endpoint().address().to_string()));
    User newUDPUser = {UDPClient};
    Rengine::Entity &entity = RType::EntityMaker::make(this->_ecs, configID);
    Rengine::Entity::size_type entityID = entity;
    std::shared_ptr<userGame> newUser = std::make_shared<userGame>(userGame{client, entityID, newUDPUser, 0});
    _users.push_back(newUser);
    this->_GameServerUDP.AddUser(newUDPUser);
    Message<Communication::TypeDetail> msg2;
    msg2.header.type = {Communication::Type::ConnexionDetail, Communication::main::ConnexionDetailPrecision::PlayableEntityInGameId};
    msg2.header.size = 0;
    msg2 << entityID;
    this->_GameServerTCP.Send(msg2, client);
    std::cout << "User added with entityID: " << entityID << std::endl;
};

void RType::Games::_handleTCPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg) {
    switch (msg.header.type.type) {
        case Communication::Type::ConnexionDetail:
            _handleConnexionTCPMessage(client, msg);
            break;
    };
};

void RType::Games::_handleEntityInfoUDPMessage(std::shared_ptr<userGame> user, Message<Communication::TypeDetail> &msg) {
    switch (msg.header.type.precision) {
        case Communication::main::EntityActionPrecision::EntityActionTypeMove:
            float x, y;
            msg >> y >> x;
            std::cout << "Entity " << user->entity << " moved to x: " << x << " y: " << y << std::endl;
            break;
        
        case Communication::main::EntityActionPrecision::EntityActionTypeShoot1:
            std::cout << "Entity " << user->entity << " shooted 1" << std::endl;
            break;
        
        case Communication::main::EntityActionPrecision::EntityActionTypeShoot2:
            std::cout << "Entity " << user->entity << " shooted 2" << std::endl;
            break;
        
        case Communication::main::EntityActionPrecision::EntityActionTypeShoot3:
            std::cout << "Entity " << user->entity << " shooted 3" << std::endl;
            break;
    }
}

void RType::Games::_handleUDPMessage(std::shared_ptr<userGame> user, Message<Communication::TypeDetail> &msg) {
    switch (msg.header.type.type) {
        case Communication::Type::EntityInfo:
            break;
    };
};
