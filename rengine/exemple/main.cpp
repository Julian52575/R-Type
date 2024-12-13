#include "ServerConnexion/ServerConnexion.hpp"

enum class CustomMsgTypes : uint32_t {
    ServerConnexionRequest,
    Message
};
void handleMessage(Message<CustomMsgTypes> &msg, const asio::ip::udp::endpoint &endpoint ,ServerConnection<CustomMsgTypes> &server) {
    std::cout << "Message received: " << msg << " From: " << endpoint << std::endl;
    if (msg.header.type == CustomMsgTypes::ServerConnexionRequest) {
        User user;
        user.endpoint = endpoint;
        server.AddUser(user);
    } else if (msg.header.type == CustomMsgTypes::Message) {
        char data[256];
        msg >> data;
        std::cout << "Data: " << data << std::endl;
    } else {
        std::cerr << "Unknown message type" << std::endl;
    }
}

int main() {
    try {
        ServerConnection<CustomMsgTypes> server(60000);

        Message<CustomMsgTypes> msg;
        msg.header.type = CustomMsgTypes::Message;
        msg.header.size = 0;
        char data[256] = "Hello from server!";
        msg << data;
        for (std::string line; std::getline(std::cin, line);) {
            if (line == "send") {
                server.SendAll(msg);
            }
            for (std::optional<std::pair<asio::ip::udp::endpoint, Message<CustomMsgTypes>>> msg = server.Receive(); msg; msg = server.Receive()) {
                handleMessage(msg->second, msg->first, server);
            }
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}