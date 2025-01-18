// //
// #include <memory>
// #include <rengine/Rengine.hpp>
// #include <rengine/RengineGraphics.hpp>
// #include <rengine/src/Graphics/GraphicManager.hpp>
// #include <rengine/src/Graphics/UserInputManager.hpp>
// #include <rengine/src/Graphics/WindowSpecs.hpp>

// #include "src/State/State.hpp"
// #include "src/State/StateManager.hpp"



// static void parseInputCloseWindow(void)
// {
//     Rengine::Graphics::GraphicManager& graphicManager = Rengine::Graphics::GraphicManagerSingletone::get();
//     std::shared_ptr<Rengine::Graphics::AWindow>& window = graphicManager.getWindow();
//     const Rengine::Graphics::UserInputManager& inputManager = graphicManager.getWindow()->getInputManager();

//     for (auto& it : inputManager) {
//         if (it.type == Rengine::Graphics::UserInputTypeWindowClosed) {
//             window->close();
//             return;
//         }
//     }
// }

// static inline void init(void)
// {
//     Rengine::Graphics::GraphicManager& graphicManager = Rengine::Graphics::GraphicManagerSingletone::get();
//     Rengine::Graphics::WindowSpecs windowSpecs;

//     windowSpecs.buildFromJson("assets/window.json");
//     graphicManager.createWindow(windowSpecs);
// }

// int main(void)
// {
//     Rengine::ECS ecs(RTYPE_ECS_ENTITY_LIMIT);
//     RType::StateManager stateManager(ecs);

//     init();
//     stateManager.setState(RType::State::StateGame);
//     while (Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->isOpen()) {
//         Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->pollInput();
//         stateManager.run();
//         parseInputCloseWindow();
//         Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->resetDeltatime();
//         Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->render();
//         Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager().clear();
//     }
//     return 0;
// }
#include "Lobby/lobby.hpp"

static Lobby *lobby = nullptr;

void signalHandler(int signum) {
    std::cout << "\n[SERVER] Signal " << signum << " received. Stopping the server..." << std::endl;
    lobby->setRunning(false);
};

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
            return 84;
        }
        uint16_t port = std::stoi(argv[1]);
        
        Lobby server(port);
        lobby = &server;
        std::signal(SIGINT, signalHandler);

        server.run();
    } catch(const std::exception& e) {
        std::cerr << "[SERVER] Uncatched Error: " << e.what() << '\n';
        return 84;
    }
}
