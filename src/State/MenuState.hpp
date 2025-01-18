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

namespace Undertale {

    enum MenuScenes {
        // Placeholder enum
        MenuScenesNA,
        MenuScenesButtonDisplay,
    };

    enum MenuStateButtons : uint8_t {
        MenuStateButtonPlay,
        MenuStateButtonQuit,
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
            MenuStateButtons _currentIndex = MenuStateButtonPlay;
            std::vector<std::shared_ptr<Rengine::Graphics::AText>> _buttonVector;
            std::shared_ptr<Rengine::Graphics::ASprite> _logo;
            std::shared_ptr<Rengine::Graphics::ASprite> _cursor;
            std::shared_ptr<Rengine::Graphics::ASound> _backgroundMusic;
            short int currentButton = 0;
            bool play = false;
    };

}  // namespace Undertale

#endif  // SRC_STATE_MENUSTATE_HPP_
