#include <memory>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Graphics/WindowSpecs.hpp>
#include <vector>

#include "Components/Liste.hpp"
#include "Config/EntityConfig.hpp"
#include "Network/EntityAction.hpp"

#include "Game/SceneManager.hpp"
#include "Game/Systems.hpp"
#include "Game/Scenes.hpp"

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

    background.addComponent<RType::Components::Sprite>(graphicManager, config.getConfig());
    background.addComponent<RType::Components::Position>(0, 0);
}

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
    Systems systems;

    sceneManager.addScene(Game, [&ecs, &sceneManager, &window, &player, &graphicManager,&systems](float deltaTime) {
        auto entities = sceneManager.getCurrentSceneEntities();
        processInputs(window, player);
        std::vector<Rengine::Entity> entities_to_make = {};
        for (auto& entity : entities) {
            systems.InputSystem(entity, deltaTime, ecs, sceneManager, graphicManager);
            systems.MovementSystem(entity, deltaTime);
            systems.renderSpriteSystem(entity);
        }
    });

    sceneManager.addScene(Menu, [&ecs, &sceneManager](float deltaTime) {
        std::cout << "Menu" << std::endl;
    });


    sceneManager.addScene(GameOver, [&ecs, &sceneManager](float deltaTime) {
        std::cout << "GameOver" << std::endl;
    });

    sceneManager.addEntityToScene(Game, background);
    sceneManager.addEntityToScene(Game, player);

    sf::Clock clock;
    while (window->isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        window->pollInput();
        sceneManager.callCurrentSceneFunction(deltaTime);
        window->render();
    }
    return 0;
}
