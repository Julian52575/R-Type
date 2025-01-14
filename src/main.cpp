//
#include <memory>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Graphics/WindowSpecs.hpp>

#include "src/State/State.hpp"
#include "src/State/StateManager.hpp"

#include "src/Config/LevelConfig.hpp"

#define RTYPE_ECS_ENTITY_LIMIT 2500

static void parseInputCloseWindow(void)
{
    Rengine::Graphics::GraphicManager& graphicManager = Rengine::Graphics::GraphicManagerSingletone::get();
    std::shared_ptr<Rengine::Graphics::AWindow>& window = graphicManager.getWindow();
    const Rengine::Graphics::UserInputManager& inputManager = graphicManager.getWindow()->getInputManager();

    for (auto& it : inputManager) {
        if (it.type == Rengine::Graphics::UserInputTypeWindowClosed) {
            window->close();
        }
    }
}

static inline void init(void)
{
    Rengine::Graphics::GraphicManager& graphicManager = Rengine::Graphics::GraphicManagerSingletone::get();
    Rengine::Graphics::WindowSpecs windowSpecs;

    windowSpecs.buildFromJson("assets/window.json");
    graphicManager.createWindow(windowSpecs);
}

int main(void)
{
    Rengine::ECS ecs(RTYPE_ECS_ENTITY_LIMIT);
    RType::StateManager stateManager(ecs);

    init();
    stateManager.setState(RType::State::StateGame);
    while (Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->isOpen()) {
        Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->pollInput();
        stateManager.run();
        parseInputCloseWindow();
        Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->resetDeltatime();
        Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->render();
        Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager().clear();
    }
    return 0;
}
