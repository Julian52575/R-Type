#include "Systems.hpp"
#include "../Components/Liste.hpp"

Systems::Systems()
{
}

Systems::~Systems()
{
}

inline static void setUpProjectile(Rengine::Entity& projectile, Rengine::Graphics::GraphicManager& graphicManager)
{
    RType::Config::EntityConfig config("assets/entities/missile1.json");

    projectile.addComponent<RType::Components::Stats>(config.getStats());
    projectile.addComponent<RType::Components::Sprite>(graphicManager, config.getSprite().getConfig());
    projectile.addComponent<RType::Components::Position>(0, 0);
}


void Systems::InputSystem(Rengine::Entity& entity,float deltatime,Rengine::ECS& ecs,RType::SceneManager<Scene>& sceneManager,Rengine::Graphics::GraphicManager& graphicManager)
{
    auto stats = entity.getComponentNoExcept<RType::Components::Stats>();
    auto pos = entity.getComponentNoExcept<RType::Components::Position>();
    auto  actionComponent = entity.getComponentNoExcept<RType::Components::Action>();
    if (!stats || !pos || !actionComponent) {
        return;
    }

    auto currentPos = pos->get().getVector2D();
    auto newPos = pos->get().getVector2D();

    for (auto it : actionComponent->get()) {
        switch (it.type) {
            // Move
            case RType::Network::EntityActionType::EntityActionTypeMove:
                newPos.x +=  it.data.moveVelocity.x * stats->get().getStats().SpeedX * deltatime;
                newPos.y +=  it.data.moveVelocity.y * stats->get().getStats().SpeedY * deltatime;
                pos->get().setX(newPos.x);
                pos->get().setY(newPos.y);
                break;
            // Shoot1 -> 3
            case RType::Network::EntityActionType::EntityActionTypeShoot1: {
                Rengine::Entity& projectile = ecs.addEntity();
                setUpProjectile(projectile, graphicManager);
                auto pos_projectile = projectile.getComponentNoExcept<RType::Components::Position>();
                pos_projectile->get().setX(currentPos.x + 10);
                pos_projectile->get().setY(currentPos.y);
                sceneManager.addEntityToScene(Game, projectile);
                break;
            }
            case RType::Network::EntityActionType::EntityActionTypeShoot2:
                std::cout << "Shoot2" << std::endl;
                break;
            case RType::Network::EntityActionType::EntityActionTypeShoot3:
                std::cout << "Shoot3" << std::endl;
                break;

            default:
                break;
        }
    }
    actionComponent->get().clear();
}

void Systems::MovementSystem(Rengine::Entity& entity,float deltatime)
{
    auto stats = entity.getComponentNoExcept<RType::Components::Stats>();
    auto pos = entity.getComponentNoExcept<RType::Components::Position>();
    auto actionComponent = entity.getComponentNoExcept<RType::Components::Action>();


    if (!stats || !pos || actionComponent) {
        return;
    }

    auto currentPos = pos->get().getVector2D();
    auto newPos = pos->get().getVector2D();

    newPos.x += stats->get().getStats().SpeedX * deltatime;
    newPos.y += stats->get().getStats().SpeedY * deltatime;

    pos->get().setX(newPos.x);
    pos->get().setY(newPos.y);
}

void Systems::renderSpriteSystem(Rengine::Entity& entity)
{
    auto pos = entity.getComponentNoExcept<RType::Components::Position>();
    auto spriteComponent = entity.getComponentNoExcept<RType::Components::Sprite>();

    if (!pos || !spriteComponent) {
        return;
    }

    spriteComponent->get().renderSprite(pos->get().getVector2D());
}