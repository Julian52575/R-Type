#pragma once
#include "../Entity/Entity.hpp"
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"
#include "../../Registry.hpp"
#include "../../Exceptions/JsonParseExecption.hpp"


#include <SFML/Graphics.hpp>
#include <utility>
#include <string>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>
#include <fstream>


class EntityMaker {
    public:
        EntityMaker();
        ~EntityMaker();
        sparse_array<Acceleration> acc;
        sparse_array<Position> pos;
        sparse_array<Rotation> rot;
        sparse_array<Scale> scale;
        sparse_array<Velocity> velo;
        sparse_array<Component::Animation> animation;
        sparse_array<Camera> camera;
        sparse_array<Music> music;
        sparse_array<Component::Parallax> parallax;
        sparse_array<Shader> shader;
        sparse_array<Sound> sound;
        sparse_array<Sprite> sprite;
        sparse_array<Text> text;
        sparse_array<Attack> attack;
        sparse_array<Group> group;
        sparse_array<Health> health;
        sparse_array<Hitbox> hitbox;
        sparse_array<Component::LifeTime> lifetime;
        sparse_array<Scripting> scripting;
        sparse_array<Clickable> clickable;
        sparse_array<Controllable> controllable;
        void EraseEntity(Entity e);
        void parseJson(Entity e, const std::string &path);
        void UpdatePosition(Entity e, float x, float y);
        void UpdateVelocity(Entity e, float x, float y);

    private:
        void parseAcceleration(Entity e, const nlohmann::json& json);
        void parsePosition(Entity e, const nlohmann::json& json);
        void parseRotation(Entity e, const nlohmann::json& json);
        void parseScale(Entity e, const nlohmann::json& json);
        void parseVelocity(Entity e, const nlohmann::json& json);
        void parseAnimations(Entity e, const nlohmann::json& json);
        void parseCamera(Entity e, const nlohmann::json& json);
        void parseMusic(Entity e, const nlohmann::json& json);
        void parseParallax(Entity e, const nlohmann::json& json);
        void parseShader(Entity e, const nlohmann::json& json);
        void parseSound(Entity e, const nlohmann::json& json);
        void parseSprite(Entity e, const nlohmann::json& json);
        void parseText(Entity e, const nlohmann::json& json);
        void parseAttack(Entity e, const nlohmann::json& json);
        void parseGroup(Entity e, const nlohmann::json& json);
        void parseHealth(Entity e, const nlohmann::json& json);
        void parseHitbox(Entity e, const nlohmann::json& json);
        void parseLifetime(Entity e, const nlohmann::json& json);
        void parseScripting(Entity e, const nlohmann::json& json);
        void parseClickable(Entity e, const nlohmann::json& json);
        void parseControllable(Entity e, const nlohmann::json& json);
        registry reg;
};

