#include <memory>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Graphics/WindowSpecs.hpp>
#include <vector>

#include "Components/Position.hpp"
#include "Components/Sprite.hpp"
#include "Components/Stats.hpp"
#include "Components/Action.hpp"
#include "Config/EntityConfig.hpp"
#include "Network/EntityAction.hpp"

#include "Game/SceneManager.hpp"

/* Window input to Component Action */
static void processInputs(std::shared_ptr<Rengine::Graphics::AWindow> &window, Rengine::Entity& player)
{
    Rengine::Graphics::UserInputManager& inputManager = window->getInputManager();
    RType::Components::Action& action = player.getComponent<RType::Components::Action>();

    for (auto it : inputManager) {
        if (it.type == Rengine::Graphics::UserInputTypeWindowClosed) {
            window->close();
            return;
        }
        action.processUserInput(it);
    }
    inputManager.clear();
}

/* Component Function */
static void processActions(const Rengine::ECS& ecs, RType::Components::Action& actionComponent, Rengine::Entity& entity)
{
    RType::Components::Stats& stats = entity.getComponent<RType::Components::Stats>();
    RType::Components::Position& pos = entity.getComponent<RType::Components::Position>();
    auto currentPos = pos.getVector2D();
    auto newPos = pos.getVector2D();

    for (auto it : actionComponent) {
        switch (it.type) {
            // Move
            case RType::Network::EntityActionType::EntityActionTypeMove:
                newPos.x +=  it.data.moveVelocity.x;
                newPos.y +=  it.data.moveVelocity.y;
                pos.setX(newPos.x);
                pos.setY(newPos.y);
                break;
            // Shoot1 -> 3
            case RType::Network::EntityActionType::EntityActionTypeShoot1:
                std::cout << "Shoot1" << std::endl;
                break;
            case RType::Network::EntityActionType::EntityActionTypeShoot2:
                std::cout << "Shoot2" << std::endl;
                break;
            case RType::Network::EntityActionType::EntityActionTypeShoot3:
                std::cout << "Shoot3" << std::endl;
                break;

            default:
                break;
        }  // switch it.type
    }  // for it : actionComponent
    actionComponent.clear();
}
static void processSprite(const Rengine::ECS& ecs, RType::Components::Sprite& spriteComponent, Rengine::Entity& entity)
{
    auto pos = entity.getComponent<RType::Components::Position>();

    spriteComponent.renderSprite(pos.getVector2D());
}

/* Set Up */
inline static void setUpEcs(Rengine::ECS& ecs)
{
    ecs.registerComponent<RType::Components::Stats>();
    ecs.registerComponent<RType::Components::Action>();
    ecs.setComponentFunction<RType::Components::Action>(processActions);
    ecs.registerComponent<RType::Components::Position>();
    ecs.registerComponent<RType::Components::Sprite>();
    ecs.setComponentFunction<RType::Components::Sprite>(processSprite);

}
inline static void setUpPlayer(Rengine::Entity& player, Rengine::Graphics::GraphicManager& graphicManager)
{
    RType::Config::EntityConfig config("assets/entities/playerDefault.json");

    player.addComponent<RType::Components::Action>(RType::Components::ActionSourceUserInput);
    player.addComponent<RType::Components::Stats>(config.getStats());
    player.addComponent<RType::Components::Sprite>(graphicManager, config.getSprite().getConfig());
    player.addComponent<RType::Components::Position>(10, 0);
}

enum Scene {
    SceneMenu,
    SceneGame,
    SceneGameOver
};

int main(void)
{
    Rengine::Graphics::WindowSpecs specs; specs.buildFromJson("assets/window.json");
    Rengine::Graphics::GraphicManager graphicManager(specs);
    std::shared_ptr<Rengine::Graphics::AWindow>& window = graphicManager.getWindow();
    Rengine::ECS ecs;
    setUpEcs(ecs);

    Rengine::Entity& player = ecs.addEntity();
    setUpPlayer(player, graphicManager);

    RType::SceneManager<Scene> sceneManager;

    sceneManager.addScene(SceneGame, [&ecs, &sceneManager, &window, &player]() {
        auto entities = sceneManager.getCurrentSceneEntities();
        processInputs(window, player);
        for (auto& entity : entities) {
            processActions(ecs, entity.getComponent<RType::Components::Action>(), entity);
            processSprite(ecs, entity.getComponent<RType::Components::Sprite>(), entity);
        }
    });

    sceneManager.addScene(SceneMenu, [&ecs, &sceneManager]() {
        std::cout << "Menu" << std::endl;
    });


    sceneManager.addScene(SceneGameOver, [&ecs, &sceneManager]() {
        std::cout << "GameOver" << std::endl;
    });

    sceneManager.addEntityToScene(SceneGame, player);

    while (window->isOpen()) {
        window->pollInput();
        sceneManager.callCurrentSceneFunction();
        window->render();
    }
    return 0;
}
