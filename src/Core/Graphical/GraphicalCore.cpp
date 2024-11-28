#include "GraphicalCore.hpp"

GraphicalCore::GraphicalCore(){
    this->pos = reg.register_component<Position>();
    this->velo = reg.register_component<Velocity>();
    this->acc = reg.register_component<Acceleration>();
    this->rot = reg.register_component<Rotation>();
    this->scale = reg.register_component<Scale>();

    this->sprite = reg.register_component<Sprite>();
    this->animation = reg.register_component<Component::Animation>();
    // this->sounds = reg.register_component<Sound>();
    // this->musics = reg.register_component<Music>();

    this->controllable = reg.register_component<Controllable>();

    reg.register_component<Health>();
    reg.register_component<Hitbox>();
    reg.register_component<Scripting>();
}

GraphicalCore::~GraphicalCore(){}

void GraphicalCore::destroy_entity(Entity e){
    em.destroyEntity(e);
}

void GraphicalCore::MakePlayer(){
    Entity e = em.createEntity();
    pos.emplace_at(e, 0, 0);
    velo.emplace_at(e, 100, 100);
    sprite.emplace_at(e, "assets/images/player.png");
    controllable.emplace_at(e);
    animation.emplace_at(e, sf::IntRect(0, 0, 160, 287), 9, 0.1f);
}

void GraphicalCore::run(){
    sf::Clock clock;

    while (this->render.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        this->render.processEvents();

        this->keyBoardInput.update(this->controllable, this->velo);
        this->movement.update(this->pos, this->velo, deltaTime);
        // this->audio.update(this->musics, this->sounds);
        this->animations.update(this->animation, this->sprite, deltaTime);
        this->render.update(this->pos, this->sprite);
    }
}