#include "Core/Rengine.hpp"
#include <cstddef>
#include <functional>

int main(void)
{
    Rengine::Rengine core;
    sf::Clock clock;
    try {
        core.makeEntity("entities/player.json");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    while (core.getRender().isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        core.getRender().processEvents();

        core.getKeyBoardInput().update(core.getEntityMaker().controllable, core.getEntityMaker().velo);
        core.getKeyBoardInput().shoot(core.getEntityMaker().controllable,
                core.getEntityMaker().pos, core.getEntityMaker().attack,deltaTime,
                std::function<void(float, float)>([&core](float x, float y) {
                    Entity e = core.makeEntity("entities/projectile.json");
                    core.getEntityMaker().UpdatePosition(e, x, y);
                })
        );
#warning Implement makePlayerAttack back

        // script.update(this->scripting);
        core.getMovement().update(core.getEntityMaker().pos, core.getEntityMaker().velo, deltaTime);
        core.getParallax().update(core.getEntityMaker().pos, core.getEntityMaker().sprite, core.getEntityMaker().parallax, deltaTime);
        core.getAnimation().update(core.getEntityMaker().animation, core.getEntityMaker().sprite, deltaTime);
        core.getCollision().update(core.getEntityMaker().pos, core.getEntityMaker().hitbox);

        // mouseInput.update(this->pos, this->velo, this->sprite);
        // this->audio.update(this->musics, this->sounds);
        std::vector<Entity> vec =  core.getLifetime().update(core.getEntityMaker().lifetime, deltaTime);
        for (auto &e : vec) {
           std::cout << "Entity " << e << " is dead" << std::endl;
           core.destroyEntity(e);
        }
        core.getCameraFollow().update(core.getEntityMaker().pos, core.getEntityMaker().camera, core.getRender().getWindow());
        core.getRender().update(core.getEntityMaker().pos, core.getEntityMaker().sprite,
            core.getEntityMaker().parallax, core.getEntityMaker().text, core.getEntityMaker().hitbox);
    }
}
