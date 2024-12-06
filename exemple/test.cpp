#include <array>
#include <iostream>
#include <asio.hpp>
#include <rengine/RengineNetwork.hpp>

struct Communication {
    enum Type : uint16_t {
        NOOP = 0,
        ConnexionDetail = 1,
        JsonDetail = 2,
        EntityAction = 3,
        EntityInfo = 4,
    };

    union main {
        enum NOOPPrecision : uint16_t {
            NOOP = 0,
        };

        enum ConnexionDetailPrecision : uint16_t {
            ClientConnexion = 0,
            ClientDisconnect = 1,
            ServerAccept = 2,
            ServerDisconnect = 3,
            ClientReady = 4,
            ClientEntityRequest = 5,
            PlayableEntityInGameId = 6,
            ServerStop = 7,
        };

        enum JsonDetailPrecision : uint16_t {
            GamePartLevel = 0,
            GamePartEntity = 1,
            GamePartPlugin = 2,
        };

        enum EntityActionPrecision : uint16_t {
            MoveUp = 0,
            MoveDown = 1,
            MoveRight = 2,
            MoveLeft = 3,
            Shoot1 = 4,
            Shoot2 = 5,
            Shoot3 = 6,
            Dodge = 7,
            Barrier = 8,
            Death = 9,
            Ultimate = 0x0A
        };

        enum EntityInfoPrecision : uint16_t {
            InfoAll = 0,
            InGameAndConfigurationId = 1,
            InfoPosition = 2,
            InfoHP = 3,
            InfoMaxHp = 4,
            InfoSpeed = 5,
            InfoMaxSpeed = 6,
            InfoDirection = 7,
        };
    };

    struct TypeDetail {
        Type type;
        uint16_t precision;
    };
};
using asio::ip::udp;
void handleMessage(Message<Communication::TypeDetail> msg, ClientConnection<Communication::TypeDetail> &client) {
  std::cout << "Message received: " << std::endl;
}

int main(int argc, char* argv[])
{
  try {
  if (argc != 3) {
    std::cerr << "Usage: client <host>" << std::endl;
    return 1;
  }
  ClientConnection<Communication::TypeDetail> client(argv[1], std::stoi(argv[2]));
  Message<Communication::TypeDetail> msg;

  msg.header.type = {Communication::Type::ConnexionDetail, Communication::main::ConnexionDetailPrecision::ClientConnexion};
  msg.header.size = 0;
  client.Send(msg);

  for (std::string line; std::getline(std::cin, line);) {
    for (std::optional<Message<Communication::TypeDetail>> msg = client.Receive(); msg; msg = client.Receive()) {
      handleMessage(*msg, client);
    }
  }
  } catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}