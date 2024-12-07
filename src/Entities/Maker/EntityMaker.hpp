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

        void setEntity(Entity e);
        void DumpEntity(Entity e);
        void DumpEntity();

        void parseJson(const std::string &path);

        void UpdatePosition(Entity e, float x, float y);
        void UpdatePosition(float x, float y);

        void UpdateVelocity(Entity e, float x, float y);
        void UpdateVelocity(float x, float y);

    private:
        void parseSprite(const nlohmann::json& json);
        void parsePosition(const nlohmann::json& json);
        void parseVelocity(const nlohmann::json& json);
        void parseControllable(const nlohmann::json& json);
        void parseAnimations(const nlohmann::json& json);
        void parseParallax(const nlohmann::json& json);
        void parseText(const nlohmann::json& json);
        void parseLifetime(const nlohmann::json& json);
        void parseCamera(const nlohmann::json& json);
        void parseHitbox(const nlohmann::json& json);
        std::shared_ptr<Entity> e;
        registry reg;

};

