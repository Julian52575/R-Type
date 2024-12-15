#include "include/game.hpp"
#include <csignal>

volatile std::sig_atomic_t gSignalStatus = 0;

void signalHandler(int signal) {
    gSignalStatus = signal;
}

void sendVeloUpdate(Game &game) {
    Message<Communication::TypeDetail> msg;
    Entity player = *game.getPlayer();

    msg.header.type = {Communication::EntityAction, Communication::main::EntityActionPrecision::Move};
    msg.header.size = 0;
    float x = game.getCore().getEntityMaker().velo[player].value().x;
    float y = game.getCore().getEntityMaker().velo[player].value().y;

    msg << y << x;
    game.getClient().Send(msg);
}

int main(int ac, char **argv)
{
    std::signal(SIGINT, signalHandler);
    try {
        sf::Clock clock;
        sf::Clock clock2;
        std::string ip;
        uint16_t port;
        if (ac == 1) {
            ip = "127.0.0.1";
            port = 4242;
        } else if (ac != 3) {
            std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
            return 1;
        } else {
            ip = argv[1];
            port = std::stoi(argv[2]);
        }
        std::cout << "Connecting to " << ip << ":" << port << std::endl;
        Game game(ip, port);
        Message<Communication::TypeDetail> msg;
        int x = 0;

        msg.header.type = {Communication::ConnexionDetail, Communication::main::ConnexionDetailPrecision::ClientConnexion};
        msg.header.size = 0;
        game.getClient().Send(msg);

        game.getCore().makeEntity("assets/entities/parallax/1.json");
        game.getCore().makeEntity("assets/entities/parallax/2.json");
        game.getCore().makeEntity("assets/entities/parallax/3.json");
        game.getCore().makeEntity("assets/entities/parallax/4.json");
        game.getCore().makeEntity("assets/entities/parallax/5.json");

        while (game.getCore().getRender().isOpen() && gSignalStatus == 0 && !game.isFinished()) {
            float deltaTime = clock.restart().asSeconds();
            float deltaTime2 = clock2.getElapsedTime().asSeconds();
            for (std::optional<Message<Communication::TypeDetail>> msg = game.getClient().Receive(); msg; msg = game.getClient().Receive()) {
                game.handleMessage(*msg);
            }
            game.getCore().getRender().processEvents();

            game.getCore().getKeyBoardInput().update(game.getCore().getEntityMaker().controllable, game.getCore().getEntityMaker().velo);
            game.getCore().getKeyBoardInput().shoot(game.getCore().getEntityMaker().controllable, game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().attack, deltaTime,
                std::function<void(float, float)>([&game](float x, float y) {
                    Message<Communication::TypeDetail> msg;

                    msg.header.type = {Communication::EntityAction, Communication::main::EntityActionPrecision::Shoot1};
                    msg.header.size = 0;

                    game.getClient().Send(msg);
                })
            );

            game.getCore().getMovement().update(game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().velo, deltaTime);
            game.getCore().getParallax().update(game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().sprite, game.getCore().getEntityMaker().parallax, deltaTime);
            game.getCore().getAnimation().update(game.getCore().getEntityMaker().animation, game.getCore().getEntityMaker().sprite, deltaTime);
            game.getCore().getCameraFollow().update(game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().camera, game.getCore().getRender().getWindow());
            game.getCore().getRender().update(game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().sprite,
                game.getCore().getEntityMaker().parallax, game.getCore().getEntityMaker().text, game.getCore().getEntityMaker().hitbox);
            if (game.getPlayer() != nullptr) {
                sendVeloUpdate(game);

            } else if (deltaTime2 > 4 && game.isFinished() == false) {
                std::cout << "Requesting playable entity" << std::endl;
                Message<Communication::TypeDetail> msg;

                msg.header.type = {Communication::ConnexionDetail, Communication::main::ConnexionDetailPrecision::RequestPlaybleEntity};
                msg.header.size = 0;
                game.getClient().Send(msg);
                clock2.restart();
            }
        }

        if (gSignalStatus != 0) {
            std::cout << "Caught signal " << gSignalStatus << ", stopping the game..." << std::endl;
        }

        Message<Communication::TypeDetail> disconectPlayer;

        disconectPlayer.header.type = {Communication::ConnexionDetail, Communication::main::ConnexionDetailPrecision::ClientDisconnect};
        disconectPlayer.header.size = 0;
        game.getClient().Send(disconectPlayer);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
