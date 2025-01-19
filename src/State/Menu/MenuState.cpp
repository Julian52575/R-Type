//
#include <exception>
#include <functional>
#include <memory>
#include <rengine/RengineGraphics.hpp>
#include <string>

#include "MenuState.hpp"
#include "src/State/AScene.hpp"
#include "src/State/GameState.hpp"
#include "src/State/Menu/CreateLobbyScene.hpp"
#include "src/State/Menu/EnterLobbyInfoScene.hpp"
#include "src/State/Menu/OptionsScene.hpp"
#include "src/State/Menu/Scenes.hpp"
#include "src/State/Menu/TitleScreenScene.hpp"
#include "src/State/State.hpp"
#include "src/State/NetworkStructs.hpp"

namespace RType {

    MenuState::MenuState(Rengine::ECS& ecs, LobbyInfo& lobbyInfo, NetworkInfo &netInfo, AccessibilitySettings& accessibilitySettings)
        : AState(ecs),
        _netInfo(netInfo),
        _lobbyInfo(lobbyInfo),
        _accessibilitySettings(accessibilitySettings)
    {
        this->initScenes();
    }
    void MenuState::registerComponents(void)
    {
    }

    State MenuState::run(void)
    {
        // WIP
        MenuScenes request;

        if (this->_scenesArray[this->_currentScene % MENUSCENES_MAX]->isLoaded() == false) {
            this->_scenesArray[this->_currentScene % MENUSCENES_MAX]->reload();
        }
        this->_scenesArray[this->_currentScene % MENUSCENES_MAX]->display();
        request = this->_scenesArray[this->_currentScene % MENUSCENES_MAX]->handleInputs();

        if (request != this->_currentScene) {
            return this->changeScene(request);
        }
        return StateMenu;
    }

    State MenuState::changeScene(MenuScenes newScene)
    {
        this->_scenesArray[this->_currentScene % MENUSCENES_MAX]->unload();
        if (newScene == MenuScenesNA) {
            return StateNA;
        } else if (newScene == MenuScenesExitToLobby) {
            return StateLobby;
        } else if (newScene == MenuScenesExitToGame) {
            return StateGame;
        }
        if (this->_scenesArray[newScene % MENUSCENES_MAX] != nullptr) {
            this->_scenesArray[newScene % MENUSCENES_MAX]->reload();
            this->_currentScene = newScene;
        } else {
            std::cerr << "Scene " << newScene << " not loaded: exiting..." << std::endl;
            return StateNA;
        }
        return StateMenu;
    }

    void MenuState::initScenes(void)
    {
        this->_scenesArray[MenuScenesEnterLobbyInfo] = std::make_shared<EnterLobbyInfoScene>(EnterLobbyInfoScene(this->_lobbyInfo));
        this->_scenesArray[MenuScenesTitleScreen] = std::make_shared<TitleScreenScene>();
        this->_scenesArray[MenuScenesOptions] = std::make_shared<OptionsScene>(this->_accessibilitySettings);
        this->_scenesArray[MenuScenesCreateLobby] = std::make_shared<CreateLobbyScene>(this->_netInfo);
    }

}  // namespace RType
