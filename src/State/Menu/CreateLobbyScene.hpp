//
#pragma once
#include <future>
#include <memory>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Graphics/ASound.hpp>
#include <rengine/src/Graphics/ASprite.hpp>
#include <rengine/src/Graphics/AText.hpp>

#include "Scenes.hpp"
#include "src/State/GameState.hpp"
#include "src/State/LobbyState.hpp"
#include "src/State/AScene.hpp"
namespace RType {

    class CreateLobbyScene : public AScene<MenuScenes> {
        public:
            CreateLobbyScene(NetworkInfo& netInfoToFill)
                : AScene(MenuScenesCreateLobby), _netInfo(netInfoToFill)  // Constructor in hpp for AScene(sceneIndex)
            {
                this->_netInfo.lobbyName.resize(14);
                this->initGraphics();
            }
            ~CreateLobbyScene(void) = default;
            void unload(void);
            void reload(void);
            void display(void);
            MenuScenes handleInputs(void);

        private:
            void initGraphics(void);
            void copyTexbox(void);
            NetworkInfo& _netInfo;

        private:
            std::shared_ptr<Rengine::Graphics::ASprite> _background;
            std::shared_ptr<Rengine::Graphics::AText> _title;
            std::shared_ptr<Rengine::Graphics::AText> _underTitle;
            std::shared_ptr<Rengine::Graphics::ASprite> _textboxShadox;
            std::shared_ptr<Rengine::Graphics::AText> _textbox;
            std::shared_ptr<Rengine::Graphics::ASprite> _enter;
            std::shared_ptr<Rengine::Graphics::ASound> _backgroundMusic;
    };

}  // namespace RType
