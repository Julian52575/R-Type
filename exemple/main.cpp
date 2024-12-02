#include "ServerConnexion/ServerConnexion.hpp"

enum class CustomMsgTypes : uint32_t {
    ServerConnexionRequest,
    ServerConnexionResponse,
    Message
};
void handleMessage(OwnedMessage<CustomMsgTypes> &msg, ServerConnection<CustomMsgTypes> &server) {
    std::cout << "Message received: " << msg.GetMessage() << "\tFrom:" << msg.GetOwner().id << std::endl;
    if (msg.GetMessage().header.type == CustomMsgTypes::Message) {
        char data[256];
        msg.GetMessage() >> data;
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
            for (std::optional<OwnedMessage<CustomMsgTypes>> msg = server.Receive(); msg; msg = server.Receive()) {
                handleMessage(*msg, server);
            }
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}