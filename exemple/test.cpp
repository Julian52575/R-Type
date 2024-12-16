#include <array>
#include <iostream>
#include <asio.hpp>
#include "UserConnexion/UserConnexion.hpp"

using asio::ip::udp;

enum class CustomMsgTypes : uint32_t {
  ServerConnexionRequest,
  Message
};
void handleMessage(Message<CustomMsgTypes> msg, ClientConnection<CustomMsgTypes> &client) {
#ifdef DEBUG
  std::cout << "Message received: " << msg << std::endl;
#endif
  if (msg.header.type == CustomMsgTypes::Message) {
    char data[256];
    msg >> data;
  #ifdef DEBUG
    std::cout << "Data: " << data << std::endl;
  #endif
  } else {
    std::cerr << "Unknown message type" << std::endl;
  }
}

int main(int argc, char* argv[])
{
  try {
  if (argc != 3) {
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
  } catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}