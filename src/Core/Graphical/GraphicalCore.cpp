#include "src/Core/Graphical/GraphicalCore.hpp"
#include <memory>
#include <utility>
#include <string>

GraphicalCore::GraphicalCore() {
    this->maker = std::make_unique<EntityMaker>();

    this->acc = reg.register_component<Acceleration>();
    this->pos = reg.register_component<Position>();
    this->rot = reg.register_component<Rotation>();
    this->scale = reg.register_component<Scale>();
    this->velo = reg.register_component<Velocity>();

    this->animation = reg.register_component<Component::Animation>();
    this->camera = reg.register_component<Camera>();
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
    this->lifetime = reg.register_component<Component::LifeTime>();
    this->scripting = reg.register_component<Scripting>();

    this->clickable = reg.register_component<Clickable>();
    this->controllable = reg.register_component<Controllable>();
}

GraphicalCore::~GraphicalCore(){}

void GraphicalCore::destroy_entity(Entity e){
    this->em.destroyEntity(e);

    this->acc.erase(e);
    this->pos.erase(e);
    this->rot.erase(e);
    this->scale.erase(e);
    this->velo.erase(e);

    this->animation.erase(e);
    this->camera.erase(e);
    this->music.erase(e);
    this->parallax.erase(e);
    this->shader.erase(e);
    this->sound.erase(e);
    this->sprite.erase(e);
    this->text.erase(e);

    this->attack.erase(e);
    this->group.erase(e);
    this->health.erase(e);
    this->hitbox.erase(e);
    this->lifetime.erase(e);
    this->scripting.erase(e);

    this->clickable.erase(e);
    this->controllable.erase(e);
}

void GraphicalCore::MakePlayer(){
    Entity e = em.createEntity();
    maker->setEntity(e);
    maker->setSprite("assets/images/player.png", std::make_pair(1.f, 1.f), std::make_pair(80, 143), sprite);
    maker->setPosition(0, 0, pos);
    maker->setVelocity(100, 100, velo);
    maker->setControllable(controllable);
    maker->setAnimations(sf::IntRect(0, 0, 160, 287), 9, 0.1f, animation);
    // maker->setCamera(camera);

}

void GraphicalCore::MakePlayerAttack(float x, float y){
    Entity e = em.createEntity();
    maker->setEntity(e);
    maker->setSprite("assets/images/laser.png", std::make_pair(0.05f, 0.05f), std::make_pair(0, 0), sprite);
    maker->setPosition(x + 25, y, pos);
    maker->setVelocity(100, 0, velo);
    maker->setLifetime(5.0f, lifetime);
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

void GraphicalCore::MakeMessage(){
    Entity e = em.createEntity();
    maker->setEntity(e);
    maker->setPosition(100, 100, pos);
    maker->setText("Hello World", "assets/fonts/arial.ttf", 24, sf::Color::White, text);
    maker->setLifetime(5.0f, lifetime);
}


void GraphicalCore::run(){
    sf::Clock clock;

    while (this->render.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        this->render.processEvents();

        this->keyBoardInput.update(this->controllable, this->velo);
        this->keyBoardInput.shoot(this->controllable, this->pos, std::function<void(float, float)>([this](float x, float y) {
            this->MakePlayerAttack(x, y);
        }));
        // script.update(this->scripting);
        this->movement.update(this->pos, this->velo, deltaTime);
        this->parallaxSystem.update(this->pos, this->sprite, this->parallax, deltaTime);
        this->animations.update(this->animation, this->sprite, deltaTime);
        this->collision.update(this->pos, this->hitbox);
        // mouseInput.update(this->pos, this->velo, this->sprite);
        // this->audio.update(this->musics, this->sounds);
        std::vector<Entity> vec =  this->lifetimeSystem.update(this->lifetime, deltaTime);
        for (auto &e : vec) {
           std::cout << "Entity " << e << " is dead" << std::endl;
           this->destroy_entity(e);
        }
        this->cameraFollow.update(this->pos, this->camera,render.getWindow());
        this->render.update(this->pos, this->sprite, this->parallax, this->text);
    }
}

