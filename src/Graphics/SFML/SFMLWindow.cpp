//
#include <SFML/Config.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <exception>
#include <memory>
#include <iostream>

#include "SFMLWindow.hpp"
#include "SFMLSprite.hpp"

namespace Rengine {
    namespace Graphics {

        void SFMLWindow::initSfKeyboardBindVector(void)
        {
            this->_sfKeyboardToUserInputBindVector = {
                // Alphabet
                {sf::Keyboard::A, {UserInputTypeKeyboardChar, 'A'}}, {sf::Keyboard::B, {UserInputTypeKeyboardChar, 'B'}},
                {sf::Keyboard::C, {UserInputTypeKeyboardChar, 'C'}}, {sf::Keyboard::D, {UserInputTypeKeyboardChar, 'D'}},
                {sf::Keyboard::E, {UserInputTypeKeyboardChar, 'E'}}, {sf::Keyboard::F, {UserInputTypeKeyboardChar, 'F'}},
                {sf::Keyboard::G, {UserInputTypeKeyboardChar, 'G'}}, {sf::Keyboard::H, {UserInputTypeKeyboardChar, 'H'}},
                {sf::Keyboard::I, {UserInputTypeKeyboardChar, 'I'}}, {sf::Keyboard::J, {UserInputTypeKeyboardChar, 'J'}},
                {sf::Keyboard::K, {UserInputTypeKeyboardChar, 'K'}}, {sf::Keyboard::L, {UserInputTypeKeyboardChar, 'L'}},
                {sf::Keyboard::M, {UserInputTypeKeyboardChar, 'M'}}, {sf::Keyboard::N, {UserInputTypeKeyboardChar, 'N'}},
                {sf::Keyboard::O, {UserInputTypeKeyboardChar, 'O'}}, {sf::Keyboard::P, {UserInputTypeKeyboardChar, 'P'}},
                {sf::Keyboard::Q, {UserInputTypeKeyboardChar, 'Q'}}, {sf::Keyboard::R, {UserInputTypeKeyboardChar, 'R'}},
                {sf::Keyboard::S, {UserInputTypeKeyboardChar, 'S'}}, {sf::Keyboard::T, {UserInputTypeKeyboardChar, 'T'}},
                {sf::Keyboard::W, {UserInputTypeKeyboardChar, 'W'}}, {sf::Keyboard::X, {UserInputTypeKeyboardChar, 'X'}},
                {sf::Keyboard::Y, {UserInputTypeKeyboardChar, 'Y'}}, {sf::Keyboard::Z, {UserInputTypeKeyboardChar, 'Z'}},
                {sf::Keyboard::Space, {UserInputTypeKeyboardChar, ' '}},
                {sf::Keyboard::Num0, {UserInputTypeKeyboardChar, '0'}}, {sf::Keyboard::Numpad0, {UserInputTypeKeyboardChar, '0'}},
                {sf::Keyboard::Num1, {UserInputTypeKeyboardChar, '1'}}, {sf::Keyboard::Numpad1, {UserInputTypeKeyboardChar, '1'}},
                {sf::Keyboard::Num2, {UserInputTypeKeyboardChar, '2'}}, {sf::Keyboard::Numpad2, {UserInputTypeKeyboardChar, '2'}},
                {sf::Keyboard::Num3, {UserInputTypeKeyboardChar, '3'}}, {sf::Keyboard::Numpad3, {UserInputTypeKeyboardChar, '3'}},
                {sf::Keyboard::Num4, {UserInputTypeKeyboardChar, '4'}}, {sf::Keyboard::Numpad4, {UserInputTypeKeyboardChar, '4'}},
                {sf::Keyboard::Num5, {UserInputTypeKeyboardChar, '5'}}, {sf::Keyboard::Numpad5, {UserInputTypeKeyboardChar, '5'}},
                {sf::Keyboard::Num6, {UserInputTypeKeyboardChar, '6'}}, {sf::Keyboard::Numpad6, {UserInputTypeKeyboardChar, '6'}},
                {sf::Keyboard::Num7, {UserInputTypeKeyboardChar, '7'}}, {sf::Keyboard::Numpad7, {UserInputTypeKeyboardChar, '7'}},
                {sf::Keyboard::Num8, {UserInputTypeKeyboardChar, '8'}}, {sf::Keyboard::Numpad8, {UserInputTypeKeyboardChar, '8'}},
                {sf::Keyboard::Num9, {UserInputTypeKeyboardChar, '9'}}, {sf::Keyboard::Numpad9, {UserInputTypeKeyboardChar, '9'}},
                // Special
                {sf::Keyboard::Escape, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialESCAPE}},
                {sf::Keyboard::Tab, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialTAB}},
                {sf::Keyboard::LShift, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialSHIFT}},
                {sf::Keyboard::RShift, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialSHIFT}},
                {sf::Keyboard::Up, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialArrowUP}},
                {sf::Keyboard::Down, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialArrowDOWN}},
                {sf::Keyboard::Left, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialArrowLEFT}},
                {sf::Keyboard::Right, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialArrowRIGHT}},
            };  // this->_sfKeyboardToUserInputBindVector
        }

        void SFMLWindow::applyWindowSpecs(void)
        {
            // Options
            unsigned int style = sf::Style::None;

            if (this->_windowSpecs.options.isBorderless == true) {
                // skip all other styles
                goto styleApply;
            }
            style |= sf::Style::Close;
            if (this->_windowSpecs.options.isFullscreen == true) {
                style |= sf::Style::Fullscreen;
            }
            if (this->_windowSpecs.options.isResizable == true) {
                style |= sf::Style::Resize;
            }
styleApply:
            if (style == sf::Style::Fullscreen) {
                this->_renderWindow.create(sf::VideoMode::getDesktopMode(), this->_windowSpecs.title, style);
            } else {
                this->_renderWindow.create({this->_windowSpecs.resolution.x, this->_windowSpecs.resolution.y}, this->_windowSpecs.title, style);
            }
            // Background color
            this->_backgroundColor.r = this->_windowSpecs.backgroundColor.x;
            this->_backgroundColor.g = this->_windowSpecs.backgroundColor.y;
            this->_backgroundColor.b = this->_windowSpecs.backgroundColor.z;
            // framerateLimit
            this->_renderWindow.setFramerateLimit(this->_windowSpecs.framerateLimit);
            // Icon
            if (this->_windowSpecs.iconImagePath == "") {
                goto skipIcon;
            }
            try {
                sf::Image icon;

                icon.loadFromFile(this->_windowSpecs.iconImagePath);
                this->_renderWindow.setIcon(this->_windowSpecs.iconSize.x, this->_windowSpecs.iconSize.y, icon.getPixelsPtr());
            } catch (std::exception& e) {;}
skipIcon:
            // Options (re)
            this->_renderWindow.setMouseCursorVisible(this->_windowSpecs.options.isCursorVisible);
            this->_renderWindow.setVerticalSyncEnabled(this->_windowSpecs.options.enableVsync);
        }

        SFMLWindow::SFMLWindow(const WindowSpecs& windowSpecs) : AWindow(windowSpecs)
        {
            this->applyWindowSpecs();
            this->initSfKeyboardBindVector();
            this->_deltatimeClock.restart();
            this->_clock.restart();
        }
        void SFMLWindow::render(void)
        {
            this->_renderWindow.display();
            this->_renderWindow.clear(this->_backgroundColor);
        }
        void SFMLWindow::addSpriteToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite,
            const Rengine::Graphics::vector2D<float>& position, bool updateAnimationFrame)
        {
            try {
                if (updateAnimationFrame == true && sprite->hasAnimation() == true) {
                    sprite->advanceFrameFromTime(this->getElapsedTimeMicroseconds());
                }
                Rengine::Graphics::SFMLSprite& sfmlSpriteWrapper = (SFMLSprite&) *sprite;
                auto type = sfmlSpriteWrapper.getSpriteSpecs().type;

                sfmlSpriteWrapper.setPosition({position.x, position.y});
                // Check SpriteSpecs.type to display the right shape
                if (type == SpriteType::SpriteTypeSprite) {
                    sf::Sprite *sprite = sfmlSpriteWrapper.getSfSprite();

                    this->_renderWindow.draw(*sprite);
                } else if (type == SpriteType::SpriteTypeCircle) {
                    sf::CircleShape *circle = sfmlSpriteWrapper.getCircle();

                    this->_renderWindow.draw(*circle);
                } else if (type == SpriteType::SpriteTypeRectangle) {
                    sf::RectangleShape* rectangle = sfmlSpriteWrapper.getRectangle();

                    this->_renderWindow.draw(*rectangle);
                }
            } catch (std::exception& e) {
                std::string msg = e.what();
                throw WindowException(msg);
            }
        }
        bool SFMLWindow::isOpen(void)
        {
            return this->_renderWindow.isOpen();
        }
        void SFMLWindow::close(void)
        {
            this->_renderWindow.close();
        }

        void SFMLWindow::pollInput(void)
        {
            sf::Event event;

            while (this->_renderWindow.pollEvent(event)) {
                Rengine::Graphics::UserInput newInput;

                newInput.type = UserInputTypeNA;
                newInput.data = {0};
                switch (event.type) {
                    // Window close
                    case sf::Event::Closed:
                        newInput.type = UserInputType::UserInputTypeWindowClosed;
                        break;
                    // Key press
                    case sf::Event::KeyPressed:
                        /*
                        newInput = getUserInputFromSfKeyboard(event.key);
                        if (newInput.type == UserInputType::UserInputTypeNA) {
                            continue;
                        }
                        */
                        // Ignored because sf::Event is worthless for keyboard.
                        // Using this->this->processKeyboardInputWithSfKeyboardInsteadOfStupidSfEventDeConStupide() instead
                        break;
                    // Mouse click
                    case sf::Event::MouseButtonPressed:
                        if (event.mouseButton.button == sf::Mouse::Button::Right) {
                            newInput.type = UserInputTypeMouseRightClick;
                        } else {
                            newInput.type = UserInputTypeMouseRightClick;
                        }
                        newInput.data.mousePosition.x = event.mouseButton.x;
                        newInput.data.mousePosition.y = event.mouseButton.y;
                        break;
                    // Joystick connection
                    case sf::Event::JoystickConnected:
                        newInput.type = UserInputTypeJoystickConnected;
                        break;
                    case sf::Event::JoystickDisconnected:
                        newInput.type = UserInputTypeJoystickDisconnected;
                        break;
                    // Joystick movement
                    case sf::Event::JoystickMoved:
                        newInput = this->processJoystickMove(event);
                        break;
                    // Joystick button
                    case sf::Event::JoystickButtonPressed:
                        newInput.type = UserInputTypeJoystickButton;
                        newInput.data.joystickButton = event.joystickButton.button;
                        break;

                    default:
                        continue;
                }  // switch(event.type)
                this->_inputManager.addInput(newInput);
            }  // while (this->_renderWindow.pollEvent(event)
            // Keyboard processing
            this->processKeyboardInputWithSfKeyboardInsteadOfStupidSfEventDeConStupide();
        }

        inline UserInput SFMLWindow::getUserInputFromSfKeyboard(const sf::Event::KeyEvent& key)
        {
            UserInput result;

            result.type = UserInputTypeNA;
            for (auto it : this->_sfKeyboardToUserInputBindVector) {
                if (it.first == key.code) {
                    result = it.second;
                    break;
                }
            }  // for
            if (result.type == UserInputTypeKeyboardChar && ('A' <= result.data.keyboardChar && result.data.keyboardChar <= 'Z')) {
                if (key.shift == false) {
                    result.data.keyboardChar += 32;
                }
            }
            return result;
        }
        inline UserInput SFMLWindow::processJoystickMove(const sf::Event& event)
        {
            UserInput input;
            //unsigned int id = event.joystickMove.joystickId;
            sf::Joystick::Axis axis = event.joystickMove.axis;
            float position = event.joystickMove.position;

            switch (axis) {
                // Left Joystick
                case sf::Joystick::X:
                    input.type = UserInputTypeJoystickLeftMove;
                    input.data.joystickPosition.x = position;
                case sf::Joystick::Y:
                    input.type = UserInputTypeJoystickLeftMove;
                    input.data.joystickPosition.y = position;
                case sf::Joystick::U:
                    input.type = UserInputTypeJoystickRightMove;
                    input.data.joystickPosition.x = position;
                case sf::Joystick::V:
                    input.type = UserInputTypeJoystickRightMove;
                    input.data.joystickPosition.y = position;
                default:
                    static bool alreadyWarn = false;

                    if (alreadyWarn == false) {
                        std::cerr << "Rengine::Graphics::SFMLWindow: Warning: Unsupported joystick axis. This warning will not appear again." << std::endl;
                        alreadyWarn = true;
                    }
                    input.type = UserInputTypeNA;
                    break;
                // Right Joystick
            }
            return input;
        }
        void SFMLWindow::processKeyboardInputWithSfKeyboardInsteadOfStupidSfEventDeConStupide(void)
        {
            // Iterate over the sfKeyboardBind vector and add an input if match is found
            for (auto& it : this->_sfKeyboardToUserInputBindVector) {
                if (sf::Keyboard::isKeyPressed(it.first) != true) {
                    continue;
                }
                // Check uppercase / lowercase
                // Check type
                if (it.second.type == UserInputTypeKeyboardChar) {
                    // Check letter
                    if ('A' <= it.second.data.keyboardChar && it.second.data.keyboardChar <= 'Z') {
                        // Check no shift
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) == false
                        && sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) == false) {
                            Rengine::Graphics::UserInput newInput = it.second;

                            newInput.data.keyboardChar += 32;
                            this->_inputManager.addInput(newInput);
                            continue;
                        }
                    }
                }
                this->_inputManager.addInput(it.second);
            }
            return;
        }

        uint64_t SFMLWindow::getElapsedTimeMicroseconds(void) const noexcept
        {
            return this->_clock.getElapsedTime().asMicroseconds();
        }
        float SFMLWindow::getElapsedTimeSeconds(void) const noexcept
        {
            return this->_clock.getElapsedTime().asSeconds();
        }

        void SFMLWindow::resetDeltatime(void) noexcept
        {
            this->_deltatimeClock.restart();
        }
        uint64_t SFMLWindow::getDeltaTimeMicroseconds(void) noexcept
        {
            return this->_deltatimeClock.getElapsedTime().asMicroseconds();
        }
        float SFMLWindow::getDeltaTimeSeconds(void) noexcept
        {
            return this->_deltatimeClock.getElapsedTime().asSeconds();
        }

    }  // namespace Rengine
}  // namespace Graphics
