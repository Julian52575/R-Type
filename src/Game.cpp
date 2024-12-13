#include "include/game.hpp"

Entity &GetOrCreateEntity(std::unordered_map<uint16_t, Entity> &entities, uint16_t id, Rengine::Rengine &core, std::string path, uint16_t configId) {
    if (entities.find(id) == entities.end()) {
        Entity e = core.makeEntity(path);
        entities.emplace(id, e);
        e.setConfigId(configId);
    }
    return entities.at(id);
}

void handleCommunicationDetail(Message<Communication::TypeDetail> &msg, Game &game) {
    switch (msg.header.type.precision) {
        case Communication::main::ConnexionDetailPrecision::PlayableEntityInGameId: {
            uint16_t id;
            msg >> id;

            Entity NewEntity = GetOrCreateEntity(game.getEntities(), id, game.getCore(), find_entity_path(3), 3);
            game.setPlayer(NewEntity);
            break;
        }

        case Communication::main::ConnexionDetailPrecision::ServerDisconnect: {
            uint16_t id;

            msg >> id;
            try {
                if (game.getEntities().find(id) != game.getEntities().end()) {
                    Entity e = game.getEntities().at(id);
                    game.getCore().getEntityMaker().EraseEntity(e);
                    game.getEntities().erase(id);
                }
            } catch (std::out_of_range &e) {
                std::cerr << "Entity with id " << id << " not found" << std::endl;
            }
            break;
        }

        default:
            std::cerr << "[CLIENT] Unknown message precision " << msg.header.type.precision << std::endl;
            break;
    }
}

void handleJsonDetail(Message<Communication::TypeDetail> &msg, Game &game) {
}

void handleEntityAction(Message<Communication::TypeDetail> &msg, Game &game) {
}

void handleEntityInfo(Message<Communication::TypeDetail> &msg, Game &game) {
    switch (msg.header.type.precision) {
        case Communication::main::EntityInfoPrecision::InfoAll: {
            uint16_t id, configId;
            float x, y;

            msg >> configId >> y >> x >> id;
            try {
                Entity e = GetOrCreateEntity(game.getEntities(), id, game.getCore(), find_entity_path(configId), configId); 
                game.getCore().getEntityMaker().UpdatePosition(e, x, y);
            } catch (std::out_of_range &e) {
                std::cerr << "Entity with id " << id << " not found" << std::endl;
            }
            break;
        }

        case Communication::main::EntityInfoPrecision::NewEntity: {
            uint16_t id, configId;
            float x, y;

            std::cout << "New entity: " << id << " " << configId << " " << x << " " << y << std::endl;
            msg >> configId >> y >> x >> id;
            Entity e = GetOrCreateEntity(game.getEntities(), id, game.getCore(), find_entity_path(configId), configId);
            game.getCore().getEntityMaker().UpdatePosition(e, x, y);
            break;
        }

        default:
            break;
    }
}


Game::Game(std::string ip, uint16_t port) : client(ip, port) {
    player = nullptr;
}

void Game::handleMessage(Message<Communication::TypeDetail> &msg) {
    switch (msg.header.type.type) {
        case Communication::ConnexionDetail:
            handleCommunicationDetail(msg, *this);
            break;
        case Communication::JsonDetail:
            handleJsonDetail(msg, *this);
            break;
        case Communication::EntityAction:
            handleEntityAction(msg, *this);
            break;
        case Communication::EntityInfo:
            handleEntityInfo(msg, *this);
            break;
        default:
            std::cerr << "[CLIENT] Unknown message type " << msg.header.type.type << std::endl;
            break;
    }
}

std::unordered_map<uint16_t, Entity> &Game::getEntities() {
    return entities;
}

Rengine::Rengine &Game::getCore() {
    return core;
}

std::shared_ptr<Entity> Game::getPlayer() {
    return player;
}

ClientConnection<Communication::TypeDetail> &Game::getClient() {
    return client;
}

void Game::setPlayer(Entity &pl) {
    this->player = std::make_shared<Entity>(pl);
    core.getEntityMaker().controllable.emplace_at(pl);
    std::cout << "Player set: " << pl << std::endl;
}

Game::~Game() {
}
