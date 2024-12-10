#include "./GraphicalCore.hpp"
#include <memory>
#include <utility>
#include <string>

Rengine::Core::Core()
{
    //this->maker = std::make_unique<EntityMaker>();
}

Rengine::Core::~Core() {}

void Rengine::Core::destroy_entity(Entity e) {
    this->_entityManager.destroyEntity(e);
    this->_entityMaker.EraseEntity(e);
}

Entity& Rengine::Core::MakeEntity(const std::string& path) {
    Entity& e = this->_entityManager.createEntity();

    std::cout << "Entity " << e << " created" << std::endl;
    this->_entityMaker.parseJson(e, path);
    return e;
}

System::movement& Rengine::Core::getMovement(void)
{
    return this->_movement;
}
System::KeyBoardInput& Rengine::Core::getKeyBoardInput(void)
{
    return this->_keyBoardInput;
}
System::Collision& Rengine::Core::getCollision(void)
{
    return this->_collision;
}
System::Parallax& Rengine::Core::getParallax(void)
{
    return this->_parallaxSystem;
}
System::Audio& Rengine::Core::getAudio(void)
{
    return this->_audio;
}
System::Animation& Rengine::Core::getAnimation(void)
{
    return this->_animations;
}
System::Render& Rengine::Core::getRender(void)
{
    return this->_render;
}
System::CameraFollow& Rengine::Core::getCameraFollow(void)
{
    return this->_cameraFollow;
}
System::Lifetime& Rengine::Core::getLifetime(void)
{
    return this->_lifetimeSystem;
}
EntityManager& Rengine::Core::getEntityManager(void)
{
    return this->_entityManager;
}
EntityMaker& Rengine::Core::getEntityMaker(void)
{
    return this->_entityMaker;
}

/*
void GraphicalCore::run(){
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

