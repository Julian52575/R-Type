#include "include/game.hpp"

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
    try {
        sf::Clock clock;
        if (ac != 4) {
            std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
            return 1;
        }
        std::string ip = argv[1];
        uint16_t port = std::stoi(argv[2]);
        Game game(ip, port);
        Message<Communication::TypeDetail> msg;
        int i = std::stoi(argv[3]);
        int x = 0;

        msg.header.type = {Communication::ConnexionDetail, Communication::main::ConnexionDetailPrecision::ClientConnexion};
        msg.header.size = 0;
        game.getClient().Send(msg);

        while (game.getCore().getRender().isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            for (std::optional<Message<Communication::TypeDetail>> msg = game.getClient().Receive(); msg; msg = game.getClient().Receive()) {
                game.handleMessage(*msg);
            }
            game.getCore().getRender().processEvents();

            game.getCore().getKeyBoardInput().update(game.getCore().getEntityMaker().controllable, game.getCore().getEntityMaker().velo);
            game.getCore().getKeyBoardInput().shoot(game.getCore().getEntityMaker().controllable,
                    game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().attack,
                    deltaTime, std::function<void(float, float)>()
                    /* std::function<void(Rengine::Core, float, float)>([core](Rengine::Core& core, float x, float y) {
                        Entity& e = core.getEntityMaker().MakeEntity("entities/projectile.json");

                        core.getEntityMaker().UpdatePosition(e, x, y);
                    }
                    ) */
            );
    #warning Implement makePlayerAttack back

            // script.update(this->scripting);
            game.getCore().getMovement().update(game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().velo, deltaTime);
            game.getCore().getParallax().update(game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().sprite, game.getCore().getEntityMaker().parallax, deltaTime);
            game.getCore().getAnimation().update(game.getCore().getEntityMaker().animation, game.getCore().getEntityMaker().sprite, deltaTime);
            game.getCore().getCollision().update(game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().hitbox);

            // mouseInput.update(this->pos, this->velo, this->sprite);
            // this->audio.update(this->musics, this->sounds);
            game.getCore().getCameraFollow().update(game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().camera, game.getCore().getRender().getWindow());
            game.getCore().getRender().update(game.getCore().getEntityMaker().pos, game.getCore().getEntityMaker().sprite,
                game.getCore().getEntityMaker().parallax, game.getCore().getEntityMaker().text, game.getCore().getEntityMaker().hitbox);
            if (game.getPlayer() != nullptr && i == 1)
                sendVeloUpdate(game);
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
