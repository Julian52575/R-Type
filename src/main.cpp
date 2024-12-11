#include "Core/Rengine.hpp"
#include <cstddef>
#include <functional>

int main(void)
{
    Rengine::Rengine core;
    sf::Clock clock;
    try {
        core.makeEntity("entities/parallax1.json");
        core.makeEntity("entities/ennemy.json");
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
        // core.getCollision().update(core.getEntityMaker().pos, core.getEntityMaker().hitbox);

        //Spawn de projectile (ennemy/player)
        core.getScript().updateAttack(core.getEntityMaker().scripting, core.getEntityMaker().attack, core.getEntityMaker().pos, deltaTime,
            std::function<void(float, float)>([&core](float x, float y) {
                Entity e = core.makeEntity("entities/projectile.json");
                core.getEntityMaker().UpdatePosition(e, x, y);
                core.getEntityMaker().UpdateVelocity(e, -100, 0);
            })
        );
        core.getKeyBoardInput().shoot(core.getEntityMaker().controllable,core.getEntityMaker().pos, core.getEntityMaker().attack,deltaTime,
            std::function<void(float, float)>([&core](float x, float y) {
                Entity e = core.makeEntity("entities/projectile.json");
                core.getEntityMaker().UpdatePosition(e, x, y);
            })
        );

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
