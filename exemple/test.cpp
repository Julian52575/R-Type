#include <array>
#include <iostream>
#include <asio.hpp>
#include "src/UserConnexion/UserConnexion.hpp"

using asio::ip::udp;

enum class CustomMsgTypes : uint32_t {
  ServerConnexionRequest,
  ServerConnexionResponse,
  Message
};
void handleMessage(Message<CustomMsgTypes> msg, ClientConnection<CustomMsgTypes> &client) {
  std::cout << "Message received: " << msg << std::endl;
  if (msg.header.type == CustomMsgTypes::Message) {
    char data[256];
    msg >> data;
    std::cout << "Data: " << data << std::endl;
  } else if (msg.header.type == CustomMsgTypes::ServerConnexionResponse) {
    unsigned int id;
    msg >> id;
    client.SetId(id);
  } else {
    std::cerr << "Unknown message type" << std::endl;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << "Usage: client <host>" << std::endl;
    return 1;
  }
  ClientConnection<CustomMsgTypes> client(argv[1], std::stoi(argv[2]));

  for (std::string line; std::getline(std::cin, line);) {
    for (std::optional<Message<CustomMsgTypes>> msg = client.Receive(); msg; msg = client.Receive()) {
      handleMessage(*msg, client);
    }
    if (line == "send") {
      Message<CustomMsgTypes> msg;
      msg.header.type = CustomMsgTypes::Message;
      msg.header.size = 0;
      char data[256] = "Hello from client!";
      msg << data;
      client.Send(msg);
    }
  }
  return 0;
}