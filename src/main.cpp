#include "Core/Rengine.hpp"
#include <cstddef>
#include <functional>

using entityToMake = std::tuple<std::string, float, float, float>;

std::vector<entityToMake> load_level(const std::string &path){
    std::ifstream file(path);
    if (!file.is_open())
        throw JsonParseException("Could not open file: " + path);
    if (file.peek() == std::ifstream::traits_type::eof())
        throw JsonParseException("File is empty: " + path);
    nlohmann::json json;
    file >> json;

    std::vector<entityToMake> entities;
    for (const auto& entity : json["entities"]) {
        float pos_x = entity["position"]["x"];
        float pos_y = entity["position"]["y"];
        float spawnTime = entity["spawntime"];
        std::string entity_path = entity["entity"];
        entities.push_back(std::make_tuple(entity_path, pos_x, pos_y, spawnTime));
    }
    return entities;
}

int main(int ac, char **av)
{
    std::vector<entityToMake> entities = load_level(av[1]);
    Rengine::Rengine core;
    sf::Clock clock;
    try {
        core.makeEntity("entities/parallax/1.json");
        core.makeEntity("entities/parallax/2.json");
        core.makeEntity("entities/parallax/3.json");
        core.makeEntity("entities/parallax/4.json");
        core.makeEntity("entities/parallax/5.json");
        core.makeEntity("entities/player.json");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    while (core.getRender().isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        core.getRender().processEvents();

        //Input utilisateur
        core.getKeyBoardInput().update(core.getEntityMaker().controllable, core.getEntityMaker().velo);

        //IA mouvement
        core.getScript().updateMovement(core.getEntityMaker().scripting, core.getEntityMaker().velo, core.getEntityMaker().pos);

        //Systeme de mouvement
        core.getMovement().update(core.getEntityMaker().pos, core.getEntityMaker().velo, deltaTime);

        //Systeme de collision
        core.getCollision().update(core.getEntityMaker().pos, core.getEntityMaker().hitbox,
        std::function<bool(Entity, Entity)>([&core](Entity e1, Entity e2) {
            if (core.getEntityMaker().group[e1].has_value() && core.getEntityMaker().group[e2].has_value()){
                if (!core.getEntityMaker().group[e1].value().has(core.getEntityMaker().group[e2].value().getGroups())) {
                    core.destroyEntity(e1);
                    core.destroyEntity(e2);
                    return true;
                }
            }
            return false;
        }));

        //Spawn de projectile
        //enneemy
        core.getScript().updateAttack(core.getEntityMaker().scripting, core.getEntityMaker().attack, core.getEntityMaker().pos, deltaTime,
            std::function<void(float, float)>([&core](float x, float y) {
                try {
                    Entity e = core.makeEntity("entities/projectile.json");
                    core.getEntityMaker().UpdatePosition(e, x, y);
                    core.getEntityMaker().UpdateGroup(e, "ennemy");
                    core.getEntityMaker().InverseEntityX(e);
                } catch (const std::exception& e) {
                    std::cerr << "Error Entity cannot be created for ennemy" << std::endl;
                }
            })
        );
        //player
        core.getKeyBoardInput().shoot(core.getEntityMaker().controllable,core.getEntityMaker().pos, core.getEntityMaker().attack,deltaTime,
            std::function<void(float, float)>([&core](float x, float y) {
                try {
                    Entity e = core.makeEntity("entities/projectile.json");
                    core.getEntityMaker().UpdatePosition(e, x+35, y+15);
                    core.getEntityMaker().UpdateGroup(e, "player");
                } catch (const std::exception& e) {
                    std::cerr << "Error Entity cannot be created for player" << std::endl;
                }
            })
        );

        for (auto &entity : entities) {
            std::get<3>(entity) -= deltaTime;
            if (std::get<3>(entity) <= 0) {
                try {
                    Entity e = core.makeEntity(std::get<0>(entity));
                    core.getEntityMaker().UpdatePosition(e, std::get<1>(entity), std::get<2>(entity));
                    core.getEntityMaker().InverseEntityX(e);
                    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
                } catch (const std::exception& e) {
                    std::cerr << "Error Ennemy cannot be created" << std::endl;
                }
            }
        }

        //regles de jeu (genre si pv < 0 alors mort ou limite de temps excpirée)
        std::vector<Entity> vec = core.getLifetime().update(core.getEntityMaker().lifetime, deltaTime);

        //destroyEntity
        for (auto &e : vec) {
            core.destroyEntity(e);
        }

        //render
        core.getParallax().update(core.getEntityMaker().pos, core.getEntityMaker().sprite, core.getEntityMaker().parallax, deltaTime);
        core.getAnimation().update(core.getEntityMaker().animation, core.getEntityMaker().sprite, deltaTime);
        core.getCameraFollow().update(core.getEntityMaker().pos, core.getEntityMaker().camera, core.getRender().getWindow());
        core.getRender().update(core.getEntityMaker().pos, core.getEntityMaker().sprite,core.getEntityMaker().parallax, core.getEntityMaker().text, core.getEntityMaker().hitbox);
    }
}