//
#pragma once

#include <vector>
#include <rengine/RengineGraphics.hpp>

#include "Scenes.hpp"
#include "src/State//AScene.hpp"
#include "src/State/State.hpp"
#include "src/Game/SceneManager.hpp"
#include "src/State/LobbyState.hpp"

namespace RType {

    enum Buttons : uint8_t {
        ButtonsIp,
        ButtonsPort,
        ButtonsPlayerJson,
        ButtonsMax
    };

    class EnterLobbyInfoScene : public AScene<MenuScenes> {
        public:
            EnterLobbyInfoScene(LobbyInfo& lobbyInfo)
                : AScene(MenuScenesEnterLobbyInfo), _lobbyInfo(lobbyInfo)  // Constructor in hpp for AScene(sceneIndex)
            {
                this->initGraphics();
            }

            ~EnterLobbyInfoScene(void) = default;
            void unload(void);
            void reload(void);
            void display(void);
            MenuScenes handleInputs(void);

        private:
            void initGraphics(void);
            MenuScenes exitToLobby(void);

        private:
            LobbyInfo& _lobbyInfo;
            // A vector of text, first is the name, second is the input
            // Can be access by using Buttons
            Buttons _currentIndex = ButtonsIp;
            std::vector<std::pair<std::shared_ptr<Rengine::Graphics::AText>, std::shared_ptr<Rengine::Graphics::AText>>> _buttonVector;
            std::shared_ptr<Rengine::Graphics::ASprite> _buttonCursor;
            std::shared_ptr<Rengine::Graphics::ASound> _backgroundMusic;

    };

}  // namespace RType
