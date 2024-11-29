#include "GraphicalCore.hpp"


GraphicalCore::GraphicalCore() {
    this->maker = std::make_unique<EntityMaker>();

    this->acc = reg.register_component<Acceleration>();
    this->pos = reg.register_component<Position>();
    this->rot = reg.register_component<Rotation>();
    this->scale = reg.register_component<Scale>();
    this->velo = reg.register_component<Velocity>();

    this->animation = reg.register_component<Component::Animation>();
    this->music = reg.register_component<Music>();
    this->parallax = reg.register_component<Component::Parallax>();
    this->shader = reg.register_component<Shader>();
    this->sound = reg.register_component<Sound>();
    this->sprite = reg.register_component<Sprite>();
    this->text = reg.register_component<Text>();

    this->attack = reg.register_component<Attack>();
    this->group = reg.register_component<Group>();
    this->health = reg.register_component<Health>();
    this->hitbox = reg.register_component<Hitbox>();
    this->lifetime = reg.register_component<LifeTime>();
    this->scripting = reg.register_component<Scripting>();

    this->clickable = reg.register_component<Clickable>();
    this->controllable = reg.register_component<Controllable>();
}

GraphicalCore::~GraphicalCore(){}

void GraphicalCore::destroy_entity(Entity e){
    em.destroyEntity(e);
}

void GraphicalCore::MakePlayer(){
    Entity e = em.createEntity();
    maker->setEntity(e);
    maker->setSprite("assets/images/player.png", std::make_pair(1.f, 1.f), std::make_pair(0, 0), sprite);
    maker->setPosition(0, 0, pos);
    maker->setVelocity(100, 100, velo);
    maker->setControllable(controllable);
    maker->setAnimations(sf::IntRect(0, 0, 160, 287), 9, 0.1f, animation);
}


void GraphicalCore::MakeBackground(){
    Entity e = em.createEntity();
    maker->setEntity(e);
    maker->setSprite("Jungle Asset Pack/Jungle Asset Pack/parallax background/plx-1.png", std::make_pair(1.f, 1.f), std::make_pair(0.f, 0.f), sprite);
    maker->setPosition(0, 0, pos);
}

void GraphicalCore::MakeParallaxBackground(const std::string& texturePath, float speed){
    Entity e = em.createEntity();
    maker->setEntity(e);
    maker->setSprite(texturePath, std::make_pair(3.f, 3.f), std::make_pair(0.f, 0.f), sprite);
    maker->setPosition(0, 0, pos);
    maker->setParallax(speed, parallax);
}


void GraphicalCore::run(){
    sf::Clock clock;

    while (this->render.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        this->render.processEvents();

        this->keyBoardInput.update(this->controllable, this->velo);
        //script.update(this->scripting);
        this->movement.update(this->pos, this->velo, deltaTime);
        this->parallaxSystem.update(this->pos, this->sprite, this->parallax, deltaTime);
        this->animations.update(this->animation, this->sprite, deltaTime);
        //collision.update(this->pos, this->velo, this->hitbox);
        //mouseInput.update(this->pos, this->velo, this->sprite);
        // this->audio.update(this->musics, this->sounds);
        //lifetime.update(this->pos, this->velo, this->acc);
        this->render.update(this->pos, this->sprite, this->parallax);
    }
}