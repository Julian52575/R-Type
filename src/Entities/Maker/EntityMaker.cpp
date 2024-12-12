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


void EntityMaker::EraseEntity(Entity e){
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
    this->scripting.erase(e); //cette ligne fais buguer le lua

    this->clickable.erase(e);
    this->controllable.erase(e);
}


void EntityMaker::parseJson(Entity e, const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw JsonParseException("Could not open file: " + path);
    if (file.peek() == std::ifstream::traits_type::eof())
        throw JsonParseException("File is empty: " + path);
    nlohmann::json json;
    file >> json;

    parseAcceleration(e, json);
    parsePosition(e, json);
    parseRotation(e, json);
    parseScale(e, json);
    parseVelocity(e, json);

    parseAnimations(e, json);
    parseCamera(e, json);
    parseMusic(e, json);
    parseParallax(e, json);
    parseShader(e, json);
    parseSound(e, json);
    parseSprite(e, json);
    parseText(e, json);

    parseAttack(e, json);
    parseGroup(e, json);
    parseHealth(e, json);
    parseHitbox(e, json);
    parseLifetime(e, json);
    parseScripting(e, json);

    parseClickable(e, json);
    parseControllable(e, json);

    file.close();
}

void EntityMaker::parseAcceleration(Entity e, const nlohmann::json& json) {
    if (!json.contains("acceleration")) {
        return;
    }
    if (!json["acceleration"].contains("x") || !json["acceleration"].contains("y")) {
        throw JsonParseException("Acceleration must have x and y");
    }
    float x = json["acceleration"]["x"];
    float y = json["acceleration"]["y"];
    this->acc.emplace_at(e, x, y);
}

void EntityMaker::parsePosition(Entity e, const nlohmann::json& json) {
    if (!json.contains("position")) {
        return;
    }
    if (!json["position"].contains("x") || !json["position"].contains("y")) {
        throw JsonParseException("Position must have x and y");
    }
    float x = json["position"]["x"];
    float y = json["position"]["y"];
    this->pos.emplace_at(e, x, y);
}

void EntityMaker::parseRotation(Entity e, const nlohmann::json& json) {
    if (!json.contains("rotation")) {
        return;
    }
    if (!json["rotation"].contains("angle")) {
        throw JsonParseException("Rotation must have an angle");
    }
    float angle = json["rotation"]["angle"];
    this->rot.emplace_at(e, angle);
}

void EntityMaker::parseScale(Entity e, const nlohmann::json& json) {
    if (!json.contains("scale")) {
        return;
    }
    if (!json["scale"].contains("x") || !json["scale"].contains("y")) {
        throw JsonParseException("Scale must have x and y");
    }
    float x = json["scale"]["x"];
    float y = json["scale"]["y"];
    this->scale.emplace_at(e, x, y);
}

void EntityMaker::parseVelocity(Entity e, const nlohmann::json& json) {
    if (!json.contains("velocity")) {
        return;
    }
    if (!json["velocity"].contains("x") || !json["velocity"].contains("y")) {
        throw JsonParseException("Velocity must have x and y");
    }
    float x = json["velocity"]["x"];
    float y = json["velocity"]["y"];
    this->velo.emplace_at(e, x, y);
}

void EntityMaker::parseAnimations(Entity e, const nlohmann::json& json) {
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
    this->animation.emplace_at(e, rect, frames, duration);
}

void EntityMaker::parseCamera(Entity e, const nlohmann::json& json) {
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
    this->camera.emplace_at(e, rect);
}

void EntityMaker::parseMusic(Entity e, const nlohmann::json& json) {
    if (!json.contains("music")) {
        return;
    }
    if (!json["music"].contains("path")) {
        throw JsonParseException("Music must have a path");
    }
    std::string path = json["music"]["path"];
    this->music.emplace_at(e, path);
}

void EntityMaker::parseParallax(Entity e, const nlohmann::json& json) {
    if (!json.contains("parallax")) {
        return;
    }
    if (!json["parallax"].contains("speed")) {
        throw JsonParseException("Parallax must have a speed");
    }
    float speed = json["parallax"]["speed"];
    this->parallax.emplace_at(e, speed);
}

void EntityMaker::parseShader(Entity e, const nlohmann::json& json) {
    if (!json.contains("shader")) {
        return;
    }
    if (!json["shader"].contains("path")) {
        throw JsonParseException("Shader must have a path");
    }
    std::string path = json["shader"]["path"];
    this->shader.emplace_at(e, path);
}

void EntityMaker::parseSound(Entity e, const nlohmann::json& json) {
    if (!json.contains("sound")) {
        return;
    }
    if (!json["sound"].contains("path")) {
        throw JsonParseException("Sound must have a path");
    }
    std::string path = json["sound"]["path"];
    this->sound.emplace_at(e, path);
}

void EntityMaker::parseSprite(Entity e, const nlohmann::json& json) {
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
    this->sprite.emplace_at(e, path, scale, origin);
}

void EntityMaker::parseText(Entity e, const nlohmann::json& json) {
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
    try { 
        this->text.emplace_at(e, font, content, size, color);
    } catch (std::exception &e) {
        return;
    }
}

void EntityMaker::parseAttack(Entity e, const nlohmann::json& json) {
    if (!json.contains("attack")) {
        return;
    }
    if (!json["attack"].contains("damage")) {
        throw JsonParseException("Attack must have damage");
    }
    if (!json["attack"].contains("cooldown")) {
        throw JsonParseException("Attack must have cooldown");
    }
    float damage = json["attack"]["damage"];
    float cooldown = json["attack"]["cooldown"];
    this->attack.emplace_at(e, damage, cooldown);
}

void EntityMaker::parseGroup(Entity e, const nlohmann::json& json) {
    if (!json.contains("group")) {
        return;
    }
    std::vector<std::string> groups = json["group"].get<std::vector<std::string>>();
    this->group.emplace_at(e, groups);
}

void EntityMaker::parseHealth(Entity e, const nlohmann::json& json) {
    if (!json.contains("health")) {
        return;
    }
    if (!json["health"].contains("max")) {
        throw JsonParseException("Health must have max");
    }
    if (!json["health"].contains("current")) {
        throw JsonParseException("Health must have current");
    }
    float max = json["health"]["max"];
    float current = json["health"]["current"];
    this->health.emplace_at(e, max, current);
}

void EntityMaker::parseHitbox(Entity e, const nlohmann::json& json) {
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
    this->hitbox.emplace_at(e, size, offset);
}


void EntityMaker::parseLifetime(Entity e, const nlohmann::json& json) {
    if (!json.contains("lifetime")) {
        return;
    }
    float time = json["lifetime"];
    if (time <= 0) {
        throw JsonParseException("Lifetime must be positive and non-zero");
    }
    this->lifetime.emplace_at(e, time);
}

void EntityMaker::parseScripting(Entity e, const nlohmann::json& json) {
    if (!json.contains("script")) {
        return;
    }
    if (!json["script"].contains("path")) {
        throw JsonParseException("Scripting must have a path");
    }
    std::string path = json["script"]["path"];
    this->scripting.emplace_at(e, path);
}

void EntityMaker::parseClickable(Entity e, const nlohmann::json& json) {
    if (!json.contains("clickable"))
        return;
    if (json["clickable"] == true)
        this->clickable.emplace_at(e);
}

void EntityMaker::parseControllable(Entity e, const nlohmann::json& json) {
    if (!json.contains("controllable"))
        return;
    if (json["controllable"] == true)
        this->controllable.emplace_at(e);
}

void EntityMaker::UpdatePosition(Entity e, float x, float y) {
    try {
        this->pos[e].value().x = x;
        this->pos[e].value().y = y;
    } catch (std::out_of_range &) {
        this->pos.emplace_at(e, x, y);
    }
    catch (std::bad_optional_access &) {
        this->pos.emplace_at(e, x, y);
    }
}

void EntityMaker::UpdateVelocity(Entity e, float x, float y) {
    try {
        this->velo[e].value().x = x;
        this->velo[e].value().y = y;
    } catch (std::out_of_range &) {
        this->velo.emplace_at(e, x, y);
    }
    catch (std::bad_optional_access &) {
        this->velo.emplace_at(e, x, y);
    }
}

void EntityMaker::UpdateGroup(Entity e, std::string group) {
    try {
        this->group[e].value().add(group);
    } catch (std::out_of_range &) {
        this->group.emplace_at(e, std::vector<std::string>{group});
    }
    catch (std::bad_optional_access &) {
        this->group.emplace_at(e, std::vector<std::string>{group});
    }
}

void EntityMaker::InverseEntityX(Entity e) {
    try {
        this->velo[e].value().x = -this->velo[e].value().x;
    } catch (std::out_of_range &) {
    }
    catch (std::bad_optional_access &) {
    }

    try {
        this->sprite[e].value().setScale(-this->sprite[e].value().getScale().x, this->sprite[e].value().getScale().y);
    } catch (std::out_of_range &) {
    }
    catch (std::bad_optional_access &) {
    }

    try{
        this->hitbox[e].value().setOffset(-(this->hitbox[e].value().getOffset().x + this->hitbox[e].value().getSize().x), this->hitbox[e].value().getOffset().y);
    } catch (std::out_of_range &) {
    }
    catch (std::bad_optional_access &) {
    }

}