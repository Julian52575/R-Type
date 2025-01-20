//
#ifndef SRC_GRAPHICS_SFML_SFMLWINDOW_HPP_
#define SRC_GRAPHICS_SFML_SFMLWINDOW_HPP_
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <optional>
#include <memory>
#include <vector>
#include <utility>
#include <string>

#include "../ASprite.hpp"
#include "../AWindow.hpp"
#include "SFMLSprite.hpp"

namespace Rengine {
    namespace Graphics {

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class SFMLWindow
        * @brief Implementation of AWindow using the SFML library.
        * See AWindow.
        */
        class SFMLWindow : public AWindow {
            public:
                explicit SFMLWindow(const WindowSpecs& windowSpecs);
                ~SFMLWindow(void) = default;
                void addSpriteToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite,
                    const Rengine::Graphics::vector2D<float>& position, bool updateAnimationFrame = true);
                void addTextToRender(const std::shared_ptr<Rengine::Graphics::AText>& text,
                    const Rengine::Graphics::vector2D<float>& position);
                void render(void);
                bool isOpen(void);
                void close(void);
                void pollInput(void);
                uint64_t getElapsedTimeMicroseconds(void) const noexcept;
                float getElapsedTimeSeconds(void) const noexcept;
                void resetDeltatime(void) noexcept;
                uint64_t getDeltaTimeMicroseconds(void) noexcept;
                float getDeltaTimeSeconds(void) noexcept;
                void setShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

            private:
                void initSfKeyboardBindVector(void);
                void applyWindowSpecs(void);
                sf::RenderWindow _renderWindow;
                sf::Color _backgroundColor;
                sf::Clock _clock;
                sf::Clock _deltatimeClock;
                std::optional<sf::Shader> _shader;
                std::vector<std::pair<sf::Keyboard::Key, UserInput>> _sfKeyboardToUserInputBindVector;
                /**
                * @return UserInput The UserInput matching the sf::Event::KeyEvent.
                * @brief Get a UserInput from a sf::Keyboard
                * Note: UserInput.UserInputType == UserInputTypeNA if no match
                */
                inline UserInput getPressedUserInputFromSfKeyboard(const sf::Event::KeyEvent& key);
                inline UserInput processJoystickMove(const sf::Event& event);
                void processKeyboardInputWithSfKeyboardInsteadOfStupidSfEventDeConStupide(void);
                inline void processJoystickMoveWithSfJoystickInsteadOfStupidSfEventDeConStupide(unsigned int joystickId);
                std::vector<unsigned int> _joystickIds = {0};
                void removeJoystickFromVector(unsigned int joystickId);
        };
    }  // namespace Graphics
}  // namespace Rengine

#endif  // SRC_GRAPHICS_SFML_SFMLWINDOW_HPP_
