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

#include <SFML/System/Clock.hpp>


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

static void InputSystem(Rengine::Entity& entity,float deltatime)
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
                newPos.x +=  it.data.moveVelocity.x * deltatime;
                newPos.y +=  it.data.moveVelocity.y * deltatime;
                pos->get().setX(newPos.x);
                pos->get().setY(newPos.y);
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
        }
    }
    actionComponent->get().clear();
}

static void renderSpriteSystem(Rengine::Entity& entity)
{
    auto pos = entity.getComponentNoExcept<RType::Components::Position>();
    auto spriteComponent = entity.getComponentNoExcept<RType::Components::Sprite>();

    if (!pos || !spriteComponent) {
        return;
    }

    spriteComponent->get().renderSprite(pos->get().getVector2D());
}

/* Set Up */
inline static void setUpEcs(Rengine::ECS& ecs)
{
    ecs.registerComponent<RType::Components::Stats>();
    ecs.registerComponent<RType::Components::Action>();
    ecs.registerComponent<RType::Components::Position>();
    ecs.registerComponent<RType::Components::Sprite>();

}

inline static void setUpPlayer(Rengine::Entity& player, Rengine::Graphics::GraphicManager& graphicManager)
{
    RType::Config::EntityConfig config("assets/entities/playerDefault.json");

    player.addComponent<RType::Components::Action>(RType::Components::ActionSourceUserInput);
    player.addComponent<RType::Components::Stats>(config.getStats());
    player.addComponent<RType::Components::Sprite>(graphicManager, config.getSprite().getConfig());
    player.addComponent<RType::Components::Position>(10, 0);
}

inline static void setUpBackground(Rengine::Entity& background, Rengine::Graphics::GraphicManager& graphicManager)
{
    RType::Config::ImageConfig config("assets/images/space_background.json");

    std::cout << "Background scale: " << config.getConfig().textureScale << std::endl;

    background.addComponent<RType::Components::Sprite>(graphicManager, config.getConfig());
    background.addComponent<RType::Components::Position>(0, 0);
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

    Rengine::Entity& background = ecs.addEntity();
    setUpBackground(background, graphicManager);

    Rengine::Entity& player = ecs.addEntity();
    setUpPlayer(player, graphicManager);


    RType::SceneManager<Scene> sceneManager;

    sceneManager.addScene(SceneGame, [&ecs, &sceneManager, &window, &player](float deltaTime) {
        auto entities = sceneManager.getCurrentSceneEntities();
        processInputs(window, player);
        for (auto& entity : entities) {
            InputSystem(entity, deltaTime);
            renderSpriteSystem(entity);
        }
    });

    sceneManager.addScene(SceneMenu, [&ecs, &sceneManager](float deltaTime) {
        std::cout << "Menu" << std::endl;
    });


    sceneManager.addScene(SceneGameOver, [&ecs, &sceneManager](float deltaTime) {
        std::cout << "GameOver" << std::endl;
    });

    sceneManager.addEntityToScene(SceneGame, background);
    sceneManager.addEntityToScene(SceneGame, player);

    sf::Clock clock;
    while (window->isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        window->pollInput();
        sceneManager.callCurrentSceneFunction(deltaTime);
        window->render();
    }
    return 0;
}
