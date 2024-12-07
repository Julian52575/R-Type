#include "./EntityMaker.hpp"
#include <utility>
#include <memory>
#include <string>

EntityMaker::EntityMaker() {
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

EntityMaker::~EntityMaker(){}

void EntityMaker::setEntity(Entity e) {
    this->e = std::make_shared<Entity>(e);
}

void EntityMaker::DumpEntity(Entity e){
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

void EntityMaker::DumpEntity(){
    DumpEntity(*e);
}

void EntityMaker::parseJson(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw JsonParseException("Could not open file: " + path);
    
    nlohmann::json json;
    file >> json;
    parseSprite(json);
    parsePosition(json);
    parseVelocity(json);
    parseControllable(json);
    parseAnimations(json);
    parseParallax(json);
    parseText(json);
    parseLifetime(json);
    parseCamera(json);
    parseHitbox(json);
}

void EntityMaker::parseSprite(const nlohmann::json& json) {
    if (!json.contains("sprite")) {
        return;
    }
    if (!json["sprite"].contains("path"))
        throw JsonParseException("Sprite must have a path");

    std::string path = json["sprite"]["path"];

    std::pair<float, float> scale = std::make_pair(1.f, 1.f);
    if (json["sprite"].contains("scale")) {
        if (!json["sprite"]["scale"].contains("x") || !json["sprite"]["scale"].contains("y")) {
            throw JsonParseException("Scale must have x and y");
        }
        scale = std::make_pair(json["sprite"]["scale"]["x"], json["sprite"]["scale"]["y"]);
    }

    std::pair<float, float> origin = std::make_pair(0.f, 0.f);
    if (json["sprite"].contains("origin")) {
        if (!json["sprite"]["origin"].contains("x") || !json["sprite"]["origin"].contains("y")) {
            throw JsonParseException("Origin must have x and y");
        }
        origin = std::make_pair(json["sprite"]["origin"]["x"], json["sprite"]["origin"]["y"]);
    }
    this->sprite.emplace_at(*e, path, scale, origin);
}

void EntityMaker::parsePosition(const nlohmann::json& json) {
    if (!json.contains("position")) {
        return;
    }
    if (!json["position"].contains("x") || !json["position"].contains("y")) {
        throw JsonParseException("Position must have x and y");
    }
    float x = json["position"]["x"];
    float y = json["position"]["y"];
    this->pos.emplace_at(*e, x, y);
}

void EntityMaker::parseVelocity(const nlohmann::json& json) {
    if (!json.contains("velocity")) {
        return;
    }
    if (!json["velocity"].contains("x") || !json["velocity"].contains("y")) {
        throw JsonParseException("Velocity must have x and y");
    }
    float x = json["velocity"]["x"];
    float y = json["velocity"]["y"];
    this->velo.emplace_at(*e, x, y);
}

void EntityMaker::parseControllable(const nlohmann::json& json) {
    if (!json.contains("controllable"))
        return;
    if (json["controllable"] == true)
        this->controllable.emplace_at(*e);
}

void EntityMaker::parseAnimations(const nlohmann::json& json) {
    if (!json.contains("animations")) {
        return;
    }
    if (!json["animations"].contains("frameRect"))
        throw JsonParseException("Animations must have a frameRect");

    if (!json["animations"]["frameRect"].contains("x") || !json["animations"]["frameRect"].contains("y") || !json["animations"]["frameRect"].contains("width") || !json["animations"]["frameRect"].contains("height"))
        throw JsonParseException("frameRect must have x, y, width and height");

    if (!json["animations"].contains("frameCount"))
        throw JsonParseException("Animations must have frameCount");

    if (!json["animations"].contains("frameDuration"))
        throw JsonParseException("Animations must have frameDuration");

    sf::IntRect rect(json["animations"]["frameRect"]["x"], json["animations"]["frameRect"]["y"], json["animations"]["frameRect"]["width"], json["animations"]["frameRect"]["height"]);
    int frames = json["animations"]["frameCount"];
    float duration = json["animations"]["frameDuration"];
    this->animation.emplace_at(*e, rect, frames, duration);
}

void EntityMaker::parseParallax(const nlohmann::json& json) {
    if (!json.contains("parallax")) {
        return;
    }
    if (!json["parallax"].contains("speed")) {
        throw JsonParseException("Parallax must have a speed");
    }
    float speed = json["parallax"]["speed"];
    this->parallax.emplace_at(*e, speed);
}

void EntityMaker::parseText(const nlohmann::json& json) {
    if (!json.contains("text")) {
        return;
    }
    if (!json["text"].contains("content") || !json["text"].contains("font") || !json["text"].contains("size") || !json["text"].contains("color")) {
        throw JsonParseException("Text must have content, font, size and color");
    }
    if (!json["text"]["color"].contains("r") || !json["text"]["color"].contains("g") || !json["text"]["color"].contains("b") || !json["text"]["color"].contains("a")) {
        throw JsonParseException("Color must have r, g, b and a");
    }
    std::string content = json["text"]["content"];
    std::string font = json["text"]["font"];
    int size = json["text"]["size"];
    sf::Color color(json["text"]["color"]["r"], json["text"]["color"]["g"], json["text"]["color"]["b"], json["text"]["color"]["a"]);
    this->text.emplace_at(*e, font, content, size, color);
}

void EntityMaker::parseLifetime(const nlohmann::json& json) {
    if (!json.contains("lifetime")) {
        return;
    }
    float time = json["lifetime"];
    if (time <= 0) {
        throw JsonParseException("Lifetime must be positive and non-zero");
    }
    this->lifetime.emplace_at(*e, time);
}

void EntityMaker::parseCamera(const nlohmann::json& json) {
    if (!json.contains("camera")) {
        return;
    }
    if (!json["camera"].contains("rect")) {
        throw JsonParseException("Camera must have a rect");
    }
    if (!json["camera"]["rect"].contains("x") || !json["camera"]["rect"].contains("y") || !json["camera"]["rect"].contains("width") || !json["camera"]["rect"].contains("height")) {
        throw JsonParseException("Rect must have x, y, width and height");
    }
    sf::FloatRect rect(json["camera"]["rect"]["x"], json["camera"]["rect"]["y"], json["camera"]["rect"]["width"], json["camera"]["rect"]["height"]);
    this->camera.emplace_at(*e, rect);
}

void EntityMaker::parseHitbox(const nlohmann::json& json) {
    if (!json.contains("hitbox")) {
        return;
    }
    if (!json["hitbox"].contains("size") || !json["hitbox"].contains("offset")) {
        throw JsonParseException("Hitbox must have size and offset");
    }
    if (!json["hitbox"]["size"].contains("x") || !json["hitbox"]["size"].contains("y") || !json["hitbox"]["offset"].contains("x") || !json["hitbox"]["offset"].contains("y")) {
        throw JsonParseException("Size and offset must have x and y");
    }
    sf::Vector2f size(json["hitbox"]["size"]["x"], json["hitbox"]["size"]["y"]);
    sf::Vector2f offset(json["hitbox"]["offset"]["x"], json["hitbox"]["offset"]["y"]);
    this->hitbox.emplace_at(*e, size, offset);
}


void EntityMaker::UpdatePosition(Entity e, float x, float y) {
    this->pos[e].value().x = x;
    this->pos[e].value().y = y;
}

void EntityMaker::UpdatePosition(float x, float y) {
    this->pos[*e].value().x = x;
    this->pos[*e].value().y = y;
}

void EntityMaker::UpdateVelocity(Entity e, float x, float y) {
    this->velo[e].value().x = x;
    this->velo[e].value().y = y;
}

void EntityMaker::UpdateVelocity(float x, float y) {
    this->velo[*e].value().x = x;
    this->velo[*e].value().y = y;
}