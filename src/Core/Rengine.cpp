#include "./Rengine.hpp"
#include <memory>
#include <utility>
#include <string>

Rengine::Rengine::Rengine()
{
    //this->maker = std::make_unique<EntityMaker>();
}

Rengine::Rengine::~Rengine() {}

void Rengine::Rengine::destroyEntity(Entity e) {
    this->_entityManager.destroyEntity(e);
    this->_entityMaker.EraseEntity(e);
}

Entity& Rengine::Rengine::makeEntity(const std::string& path) {
    Entity& e = this->_entityManager.createEntity();

    std::cout << "Entity " << e << " created" << std::endl;
    this->_entityMaker.parseJson(e, path);
    return e;
}

System::movement& Rengine::Rengine::getMovement(void)
{
    return this->_movement;
}
System::KeyBoardInput& Rengine::Rengine::getKeyBoardInput(void)
{
    return this->_keyBoardInput;
}
System::Collision& Rengine::Rengine::getCollision(void)
{
    return this->_collision;
}
System::Parallax& Rengine::Rengine::getParallax(void)
{
    return this->_parallaxSystem;
}
System::Audio& Rengine::Rengine::getAudio(void)
{
    return this->_audio;
}
System::Animation& Rengine::Rengine::getAnimation(void)
{
    return this->_animations;
}
System::Render& Rengine::Rengine::getRender(void)
{
    return this->_render;
}
System::CameraFollow& Rengine::Rengine::getCameraFollow(void)
{
    return this->_cameraFollow;
}
System::Lifetime& Rengine::Rengine::getLifetime(void)
{
    return this->_lifetimeSystem;
}
EntityManager& Rengine::Rengine::getEntityManager(void)
{
    return this->_entityManager;
}
EntityMaker& Rengine::Rengine::getEntityMaker(void)
{
    return this->_entityMaker;
}

/*
void GraphicalRengine::run(){
    sf::Clock clock;

    while (this->render.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        this->render.processEvents();

        this->keyBoardInput.update(maker->controllable, maker->velo);
        this->keyBoardInput.shoot(maker->controllable, maker->pos, maker->attack, deltaTime,
        std::function<void(float, float)>([this](float x, float y) {
            Entity e = this->MakeEntity("entities/projectile.json");
            this->maker->UpdatePosition(e, x, y);
        }));
        // script.update(this->scripting);
        this->movement.update(maker->pos, maker->velo, deltaTime);
        this->parallaxSystem.update(maker->pos, maker->sprite, maker->parallax, deltaTime);
        this->animations.update(maker->animation, maker->sprite, deltaTime);
        this->collision.update(maker->pos, maker->hitbox);
        // mouseInput.update(this->pos, this->velo, this->sprite);
        // this->audio.update(this->musics, this->sounds);
        std::vector<Entity> vec =  this->lifetimeSystem.update(maker->lifetime, deltaTime);
        for (auto &e : vec) {
           std::cout << "Entity " << e << " is dead" << std::endl;
           this->destroy_entity(e);
        }
        this->cameraFollow.update(maker->pos, maker->camera,render.getWindow());
        this->render.update(maker->pos, maker->sprite, maker->parallax, maker->text,maker->hitbox);
    }
}*/

