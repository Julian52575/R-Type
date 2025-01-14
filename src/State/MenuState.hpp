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

#include "AState.hpp"
#include "State.hpp"
#include "LobbyState.hpp"

#include "src/Config/LevelConfigResolver.hpp"
#include "src/Components/Buff.hpp"
#include "src/Components/Sprite.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Action.hpp"
#include "src/Components/Components.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Relationship.hpp"

namespace RType {

    enum RTypeMenuScenes {
        // Placeholder enum
        RTypeMenuScenesNA,
        RTypeMenuScenesAttractMode,
        RTypeMenuScenesSelectLobby,
        RTypeMenuScenesButtonDisplay,
        RTypeMenuScenesLoadLobbyInfoAndExit
    };

    enum MenuStateButtons : uint8_t {
        MenuStateButtonsIp,
        MenuStateButtonsPort,
        MenuStateButtonsPlayerJson,
        MenuStateButtonsMax
    };

    class MenuState : public AState {
        public:
            MenuState(Rengine::ECS& ecs);
            ~MenuState(void) = default;
            /*
            * @fn registerComponents
            * @brief Registers the neccessary component to the ECS.
            */
            void registerComponents(void);
            /*
            * @fn run
            * @return State The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            State run(void);
            const LobbyInfo& getLobbyInfo(void) const noexcept;

        private:
            /**
            * @brief init the scenes
            */
            void initScenes(void);
            /**
            * @brief Handle the window input
            */
            void buttonDisplayHandleInput(void);
            /**
            * @brief init the buttons
            */
            void buttonDisplayInitButtons(void);
            /**
            * @brief Main function for scene buttonDisplay
            */
            friend State buttonDisplay(MenuState& menu);

        private:
            /**
            * @brief Main function for scene loadLobbyInfoAndExit
            */
            friend State loadLobbyInfoAndExit(MenuState& menu);

        private:
            LobbyInfo _lobbyInfo;
            MenuStateButtons _currentIndex = MenuStateButtonsIp;
            // A vector of text, first is the name, second is the input
            // Can be access by using MenuStateButtons
            std::vector<std::pair<std::shared_ptr<Rengine::Graphics::AText>, std::shared_ptr<Rengine::Graphics::AText>>> _buttonVector;
            std::shared_ptr<Rengine::Graphics::ASprite> _cursor;
            std::shared_ptr<Rengine::Graphics::ASound> _backgroundMusic;
    };

}  // namespace RType

#endif  // SRC_STATE_MENUSTATE_HPP_
