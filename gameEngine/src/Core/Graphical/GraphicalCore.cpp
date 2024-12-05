#include "src/Core/Graphical/GraphicalCore.hpp"
#include <memory>
#include <utility>
#include <string>

GraphicalCore::GraphicalCore() {
    this->maker = std::make_unique<EntityMaker>();

    maker->acc = reg.register_component<Acceleration>();
    maker->pos = reg.register_component<Position>();
    maker->rot = reg.register_component<Rotation>();
    maker->scale = reg.register_component<Scale>();
    maker->velo = reg.register_component<Velocity>();

    maker->animation = reg.register_component<Component::Animation>();
    maker->camera = reg.register_component<Camera>();
    maker->music = reg.register_component<Music>();
    maker->parallax = reg.register_component<Component::Parallax>();
    maker->shader = reg.register_component<Shader>();
    maker->sound = reg.register_component<Sound>();
    maker->sprite = reg.register_component<Sprite>();
    maker->text = reg.register_component<Text>();

    maker->attack = reg.register_component<Attack>();
    maker->group = reg.register_component<Group>();
    maker->health = reg.register_component<Health>();
    maker->hitbox = reg.register_component<Hitbox>();
    maker->lifetime = reg.register_component<Component::LifeTime>();
    maker->scripting = reg.register_component<Scripting>();

    maker->clickable = reg.register_component<Clickable>();
    maker->controllable = reg.register_component<Controllable>();
}

GraphicalCore::~GraphicalCore(){}

void GraphicalCore::destroy_entity(Entity e){
    this->em.destroyEntity(e);

    maker->acc.erase(e);
    maker->pos.erase(e);
    maker->rot.erase(e);
    maker->scale.erase(e);
    maker->velo.erase(e);

    maker->animation.erase(e);
    maker->camera.erase(e);
    maker->music.erase(e);
    maker->parallax.erase(e);
    maker->shader.erase(e);
    maker->sound.erase(e);
    maker->sprite.erase(e);
    maker->text.erase(e);

    maker->attack.erase(e);
    maker->group.erase(e);
    maker->health.erase(e);
    maker->hitbox.erase(e);
    maker->lifetime.erase(e);
    maker->scripting.erase(e);

    maker->clickable.erase(e);
    maker->controllable.erase(e);
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
