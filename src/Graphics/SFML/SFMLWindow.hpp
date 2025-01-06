//
#ifndef _SRC_GRAPHICS_SFML_SFMLWINDOW_HPP_
#define _SRC_GRAPHICS_SFML_SFMLWINDOW_HPP_
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <optional>
#include <memory>

#include "../AWindow.hpp"
#include "SFMLSprite.hpp"

namespace Rengine {
    namespace Graphics {

        class SFMLWindow : public AWindow {
            public:
                SFMLWindow(const sf::VideoMode& videoMode, const std::string& windowTitle);
                ~SFMLWindow(void) = default;
                void addSpriteToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite,
                    const Rengine::Graphics::vector2D<float>& position, bool updateAnimationFrame = true);
                void render(void);
                bool isOpen(void);
                void close(void);
                void pollInput(void);
                uint64_t getElapsedTimeMicroseconds(void) const noexcept;
                float getElapsedTimeSeconds(void) const noexcept;

            private:
                sf::RenderWindow _renderWindow;
                sf::Clock _clock;
                std::vector<std::pair<sf::Keyboard::Key, UserInput>> _sfKeyboardToUserInputBindVector;
                /**
                * @return UserInput The UserInput matching the sf::Event::KeyEvent.
                * @brief Get a UserInput from a sf::Keyboard
                * Note: UserInput.UserInputType == UserInputTypeNA if no match
                */
                inline UserInput getUserInputFromSfKeyboard(const sf::Event::KeyEvent& key);
                inline UserInput processJoystickMove(const sf::Event& event);
        };

    }  // namespace Rengine
}  // namespace Graphics

#endif
