#include "src/Entities/Maker/EntityMaker.hpp"
#include <utility>
#include <memory>
#include <string>

EntityMaker::EntityMaker() {}

EntityMaker::~EntityMaker(){}

void EntityMaker::setEntity(Entity e) {
    this->e = std::make_shared<Entity>(e);
}

void EntityMaker::parseJson(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file" + path);
    }
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
    if (!json["sprite"].contains("path")) {
        throw std::runtime_error("Sprite must have a path");
    }
    std::string path = json["sprite"]["path"];

    std::pair<float, float> scale = std::make_pair(1.f, 1.f);
    if (json["sprite"].contains("scale")) {
        if (!json["sprite"]["scale"].contains("x") || !json["sprite"]["scale"].contains("y")) {
            throw std::runtime_error("Scale must have x and y");
        }
        scale = std::make_pair(json["sprite"]["scale"]["x"], json["sprite"]["scale"]["y"]);
    }

    std::pair<float, float> origin = std::make_pair(0.f, 0.f);
    if (json["sprite"].contains("origin")) {
        if (!json["sprite"]["origin"].contains("x") || !json["sprite"]["origin"].contains("y")) {
            throw std::runtime_error("Origin must have x and y");
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
        throw std::runtime_error("Position must have x and y");
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
        throw std::runtime_error("Velocity must have x and y");
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
        throw std::runtime_error("Animations must have a frameRect");
    
    if (!json["animations"]["frameRect"].contains("x") || !json["animations"]["frameRect"].contains("y") || !json["animations"]["frameRect"].contains("width") || !json["animations"]["frameRect"].contains("height"))
        throw std::runtime_error("frameRect must have x, y, width and height");
    
    if (!json["animations"].contains("frameCount"))
        throw std::runtime_error("Animations must have frameCount");
    
    if (!json["animations"].contains("frameDuration"))
        throw std::runtime_error("Animations must have frameDuration");

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
        throw std::runtime_error("Parallax must have a speed");
    }
    float speed = json["parallax"]["speed"];
    this->parallax.emplace_at(*e, speed);
}

void EntityMaker::parseText(const nlohmann::json& json) {
    if (!json.contains("text")) {
        return;
    }
    if (!json["text"].contains("content") || !json["text"].contains("font") || !json["text"].contains("size") || !json["text"].contains("color")) {
        throw std::runtime_error("Text must have content, font, size and color");
    }
    if (!json["text"]["color"].contains("r") || !json["text"]["color"].contains("g") || !json["text"]["color"].contains("b") || !json["text"]["color"].contains("a")) {
        throw std::runtime_error("Color must have r, g, b and a");
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
        throw std::runtime_error("Lifetime must be positive and non-zero");
    }
    this->lifetime.emplace_at(*e, time);
}

void EntityMaker::parseCamera(const nlohmann::json& json) {
    if (!json.contains("camera")) {
        return;
    }
    if (!json["camera"].contains("rect")) {
        throw std::runtime_error("Camera must have a rect");
    }
    if (!json["camera"]["rect"].contains("x") || !json["camera"]["rect"].contains("y") || !json["camera"]["rect"].contains("width") || !json["camera"]["rect"].contains("height")) {
        throw std::runtime_error("Rect must have x, y, width and height");
    }
    sf::FloatRect rect(json["camera"]["rect"]["x"], json["camera"]["rect"]["y"], json["camera"]["rect"]["width"], json["camera"]["rect"]["height"]);
    this->camera.emplace_at(*e, rect);
}

void EntityMaker::parseHitbox(const nlohmann::json& json) {
    if (!json.contains("hitbox")) {
        return;
    }
    if (!json["hitbox"].contains("size") || !json["hitbox"].contains("offset")) {
        throw std::runtime_error("Hitbox must have size and offset");
    }
    if (!json["hitbox"]["size"].contains("x") || !json["hitbox"]["size"].contains("y") || !json["hitbox"]["offset"].contains("x") || !json["hitbox"]["offset"].contains("y")) {
        throw std::runtime_error("Size and offset must have x and y");
    }
    sf::Vector2f size(json["hitbox"]["size"]["x"], json["hitbox"]["size"]["y"]);
    sf::Vector2f offset(json["hitbox"]["offset"]["x"], json["hitbox"]["offset"]["y"]);
    this->hitbox.emplace_at(*e, size, offset);
}
