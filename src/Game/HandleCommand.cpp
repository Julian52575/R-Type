#include "./game.hpp"

void Games::_handleConnexionTCPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg) {
    asio::ip::udp::endpoint UDPClient;
    msg >> UDPClient;

    UDPClient.address(asio::ip::make_address(client->getSocket().remote_endpoint().address().to_string()));
    std::shared_ptr<userGame> newUser = std::make_shared<userGame>(userGame{client, std::move(UDPClient), 0});
    _users.push_back(newUser);
};

void Games::_handleTCPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg) {
    switch (msg.header.type.type) {
        case Communication::Type::ConnexionDetail:
            _handleConnexionTCPMessage(client, msg);
            break;
    };
};
