#include "./game.hpp"
#include "src/Game/EntityMaker.hpp"
#include "src/Network/EntityAction.hpp"
#include "src/Components/Action.hpp"

void RType::Games::_handleConnexionTCPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg) {
    asio::ip::udp::endpoint UDPClient;
    uint16_t configID;
    msg >> UDPClient >> configID;

    UDPClient.address(asio::ip::make_address(client->getSocket().remote_endpoint().address().to_string()));
    User newUDPUser = {UDPClient};
    Rengine::Entity &entity = RType::EntityMaker::make(this->_ecs, configID);
    entity.addComponent<RType::Components::Action>(RType::Components::ActionSource::ActionSourceUserInput);
    entity.addComponent<RType::Components::Velocity>(0, 0);
    entity.setComponentsDestroyFunction(
       [](Rengine::Entity& en) {
            en.removeComponentNoExcept<RType::Components::Action>();
            en.removeComponent<RType::Components::Velocity>();
        }
    );
    Rengine::Entity::size_type entityID = entity;
    std::shared_ptr<userGame> newUser = std::make_shared<userGame>(userGame{client, entityID, newUDPUser});
    _users.push_back(newUser);
    this->_GameServerUDP.AddUser(newUDPUser);
    Message<Communication::TypeDetail> msg2;
    msg2.header.type = {Communication::Type::ConnexionDetail, Communication::main::ConnexionDetailPrecision::PlayableEntityInGameId};
    msg2.header.size = 0;
    msg2 << entityID;
    this->_GameServerTCP.Send(msg2, client);
};

void RType::Games::_handleTCPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg) {
    switch (msg.header.type.type) {
        case Communication::Type::ConnexionDetail:
            _handleConnexionTCPMessage(client, msg);
            break;
    };
};

void RType::Games::_handleEntityInfoUDPMessage(std::shared_ptr<userGame> user, Message<Communication::TypeDetail> &msg, Rengine::SparseArray<RType::Components::Action>& actions) {
    switch (msg.header.type.precision) {
        case Communication::main::EntityActionPrecision::EntityActionTypeMove: {
            float x, y;
            msg >> y >> x;
            Network::EntityAction act;
            act.type = Network::EntityActionTypeMove;
            act.data.moveVelocity = {x, y};
            actions[user->entity]->processAction(act);
            break;
        }

        case Communication::main::EntityActionPrecision::EntityActionTypeShoot1: {
            Network::EntityAction act;
            act.type = Network::EntityActionTypeShoot1;
            actions[user->entity]->processAction(act);
            break;
        }

        case Communication::main::EntityActionPrecision::EntityActionTypeShoot2: {
            Network::EntityAction act;
            act.type = Network::EntityActionTypeShoot2;
            actions[user->entity]->processAction(act);
            break;
        }

        case Communication::main::EntityActionPrecision::EntityActionTypeShoot3: {
            Network::EntityAction act;
            act.type = Network::EntityActionTypeShoot3;
            actions[user->entity]->processAction(act);
            break;
        }
    }
}

void RType::Games::_handleUDPMessage(std::shared_ptr<userGame> user, Message<Communication::TypeDetail> &msg, Rengine::SparseArray<RType::Components::Action>& actions) {
    switch (msg.header.type.type) {
        case Communication::Type::EntityAction:
            _handleEntityInfoUDPMessage(user, msg, actions);
            break;
    };
};
