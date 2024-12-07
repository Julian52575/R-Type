#include "./GraphicalCore.hpp"
#include <memory>
#include <utility>
#include <string>

GraphicalCore::GraphicalCore() {
    this->maker = std::make_unique<EntityMaker>();
}

GraphicalCore::~GraphicalCore(){}

void GraphicalCore::destroy_entity(Entity e){
    this->em.destroyEntity(e);
    this->maker->DumpEntity(e);
}

void GraphicalCore::MakeEntity(const std::string path){
    Entity e = em.createEntity();
    maker->setEntity(e);
    maker->parseJson(path);
}

void GraphicalCore::MakeParallaxBackground(const std::string& texturePath, float speed){
    Entity e = em.createEntity();
    maker->setEntity(e);
    // maker->setSprite(texturePath, std::make_pair(3.f, 3.f), std::make_pair(0.f, 0.f), sprite);
    // maker->setPosition(0, 0, pos);
    // maker->setParallax(speed, parallax);
}

void GraphicalCore::run(){
    sf::Clock clock;

    while (this->render.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        this->render.processEvents();

        this->keyBoardInput.update(maker->controllable, maker->velo);
        // this->keyBoardInput.shoot(maker->controllable, maker->pos, std::function<void(float, float)>([this](float x, float y) {
        //     this->MakePlayerAttack(x, y);
        // }));
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
}

