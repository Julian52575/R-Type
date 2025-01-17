//
#include <exception>
#include <functional>
#include <memory>
#include <rengine/RengineGraphics.hpp>
#include <string>

#include "MenuState.hpp"
#include "src/State/Menu/EnterLobbyInfoScene.hpp"
#include "src/State/Menu/Scenes.hpp"
#include "src/State/State.hpp"

namespace RType {

    MenuState::MenuState(Rengine::ECS& ecs) : AState(ecs)
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
        if (newScene == MenuScenesExitToLobby) {
            return StateLobby;
        }
        this->_scenesArray[newScene % MENUSCENES_MAX]->reload();
        this->_currentScene = newScene;
        return StateMenu;
    }

    void MenuState::initScenes(void)
    {
        this->_scenesArray[MenuScenesEnterLobbyInfo] = std::make_shared<EnterLobbyInfoScene>(EnterLobbyInfoScene(this->_lobbyInfo));
    }

    /*              Getter              */
    const LobbyInfo& MenuState::getLobbyInfo(void) const noexcept
    {
        return this->_lobbyInfo;
    }
    AccessibilitySettings& MenuState::getAccessibilitySettings(void) noexcept
    {
        return this->_accessibilitySettings;
    }

}  // namespace RType
