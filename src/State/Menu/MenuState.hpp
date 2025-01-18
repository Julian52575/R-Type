//
#ifndef SRC_STATE_MENUSTATE_HPP_
#define SRC_STATE_MENUSTATE_HPP_

#include <memory>
#include <rengine/Rengine.hpp>
#include <rengine/src/Graphics/ASound.hpp>
#include <rengine/src/Graphics/ASprite.hpp>
#include <rengine/src/Graphics/AText.hpp>
#include <utility>
#include <vector>

#include "src/State/AScene.hpp"
#include "src/State/AState.hpp"
#include "src/State/LobbyState.hpp"
#include "src/State/Menu/Scenes.hpp"
#include "src/State/State.hpp"
#include "src/State/Menu/Scenes.hpp"
#include "src/Config/LevelConfigResolver.hpp"
#include "src/Game/AccessibilitySettings.hpp"

namespace RType {

    class MenuState : public AState {
        public:
            MenuState(Rengine::ECS& ecs);
            ~MenuState(void) = default;
            /**
            * @fn registerComponents
            * @brief Registers the neccessary component to the ECS.
            */
            void registerComponents(void);
            /**
            * @fn run
            * @return State The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            State run(void);
            const LobbyInfo& getLobbyInfo(void) const noexcept;
            /**
             * @fn getAccessibilitySettings
             * @return The accessibility settings set by the player.
             * @brief Return the accessibility settings.
             */
            AccessibilitySettings& getAccessibilitySettings(void) noexcept;

        private:
            /**
            * @brief Change the scene
            */
            State changeScene(MenuScenes newScene);
            /**
            * @brief init the scenes
            */
            void initScenes(void);

        // These variables are passed as parameters to the scenes.
        private:
            LobbyInfo _lobbyInfo;
            std::string _newRoomName;
            AccessibilitySettings _accessibilitySettings;

        private:
            MenuScenes _currentScene = MenuScenesTitleScreen;
            std::shared_ptr<Rengine::Graphics::ASound> _backgroundMusic;
            std::array<std::shared_ptr<AScene<MenuScenes>>, static_cast<std::size_t>(MENUSCENES_MAX)> _scenesArray;
    };

}  // namespace RType

#endif  // SRC_STATE_MENUSTATE_HPP_
