//
#include <SFML/Audio/Music.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cmath>
#include <exception>
#include <memory>
#include <iostream>
#include <valarray>
#include <vector>
#include <string>

#include "../ASound.hpp"
#include "../AText.hpp"
#include "../ASprite.hpp"
#include "SFMLSound.hpp"
#include "SFMLText.hpp"
#include "SFMLWindow.hpp"
#include "SFMLSprite.hpp"
#include "numbers"

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
                {sf::Keyboard::Period, {UserInputTypeKeyboardChar, '.'}},
                {sf::Keyboard::Add, {UserInputTypeKeyboardChar, '+'}},
                {sf::Keyboard::Subtract, {UserInputTypeKeyboardChar, '-'}},
                {sf::Keyboard::Multiply, {UserInputTypeKeyboardChar, '*'}},
                {sf::Keyboard::Divide, {UserInputTypeKeyboardChar, '/'}},
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
                {sf::Keyboard::Enter, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialENTER}},
                {sf::Keyboard::LShift, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialSHIFT}},
                {sf::Keyboard::RShift, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialSHIFT}},
                {sf::Keyboard::Delete, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialDELETE}},
                {sf::Keyboard::Backspace, {UserInputTypeKeyboardSpecial, UserInputKeyboardSpecialBACKSPACE}},
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
            // Joystick threshold
            this->_renderWindow.setJoystickThreshold(this->_windowSpecs.joystickThreshold);
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
            if (sprite == nullptr) {
                return;
            }
            try {
                if (updateAnimationFrame == true && sprite->hasAnimation() == true) {
                    sprite->advanceFrameFromTime(this->getElapsedTimeMicroseconds());
                }
                Rengine::Graphics::SFMLSprite& sfmlSpriteWrapper = (SFMLSprite&) *sprite;
                sf::Shader* shader = this->_shader.has_value() ? &this->_shader.value() : nullptr;
                auto type = sfmlSpriteWrapper.getSpriteSpecs().type;

                sfmlSpriteWrapper.setPosition({position.x, position.y});
                // Check SpriteSpecs.type to display the right shape
                if (type == SpriteType::SpriteTypeSprite) {
                    sf::Sprite *sprite = sfmlSpriteWrapper.getSfSprite();

                    this->_renderWindow.draw(*sprite, shader);
                } else if (type == SpriteType::SpriteTypeCircle) {
                    sf::CircleShape *circle = sfmlSpriteWrapper.getCircle();

                    this->_renderWindow.draw(*circle, shader);
                } else if (type == SpriteType::SpriteTypeRectangle) {
                    sf::RectangleShape* rectangle = sfmlSpriteWrapper.getRectangle();

                    this->_renderWindow.draw(*rectangle, shader);
                }
            } catch (std::exception& e) {
                std::string msg = e.what();

                throw WindowException(msg);
            }
        }
        void SFMLWindow::addTextToRender(const std::shared_ptr<Rengine::Graphics::AText>& text,
                    const Rengine::Graphics::vector2D<float>& position)
        {
            try {
                sf::Vector2f sfmlPos = {position.x, position.y};
                SFMLText& sfmlTextWrapper = (SFMLText &) *text;
                sf::Text& text = sfmlTextWrapper.getSfText();

                text.setPosition(sfmlPos);
                this->_renderWindow.draw(text);
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
                    // For CharPressed / SpecialPressed
                    case sf::Event::KeyPressed:
                        newInput = getPressedUserInputFromSfKeyboard(event.key);
                        if (newInput.type == UserInputType::UserInputTypeNA) {
                            continue;
                        }
                        break;
                    // Mouse click
                    case sf::Event::MouseButtonPressed:
                        if (event.mouseButton.button == sf::Mouse::Button::Right) {
                            newInput.type = UserInputTypeMouseRightClick;
                        } else {
                            newInput.type = UserInputTypeMouseLeftClick;
                        }
                        newInput.data.mousePosition.x = event.mouseButton.x;
                        newInput.data.mousePosition.y = event.mouseButton.y;
                        break;
                    // Joystick connection
                    case sf::Event::JoystickConnected:
                        newInput.type = UserInputTypeJoystickConnected;
                        newInput.data.joystickInput.joystickId = event.joystickConnect.joystickId;
                        this->_joystickIds.push_back(event.joystickConnect.joystickId);
                        break;
                    case sf::Event::JoystickDisconnected:
                        newInput.type = UserInputTypeJoystickDisconnected;
                        newInput.data.joystickInput.joystickId = event.joystickConnect.joystickId;
                        this->removeJoystickFromVector(event.joystickConnect.joystickId);
                        break;
                    // Joystick movement
                    case sf::Event::JoystickMoved:
                        // newInput = this->processJoystickMove(event);
                        // break;
                        // Moved to processJoystickMoveWithSfJoystickInsteadOfStupidSfEventDeConStupide
                        continue;
                    // Joystick button
                    case sf::Event::JoystickButtonPressed:
                        newInput.type = UserInputTypeJoystickButton;
                        newInput.data.joystickInput.data.joystickButton = event.joystickButton.button;
                        newInput.data.joystickInput.joystickId = event.joystickButton.joystickId;
                        break;

                    default:
                        continue;
                }  // switch(event.type)
                if (newInput.type != UserInputTypeNA) {
                    this->_inputManager.addInput(newInput);
                }
            }  // while (this->_renderWindow.pollEvent(event)
            // Keyboard processing
            this->processKeyboardInputWithSfKeyboardInsteadOfStupidSfEventDeConStupide();
            for (auto it : this->_joystickIds) {
                this->processJoystickMoveWithSfJoystickInsteadOfStupidSfEventDeConStupide(it);
            }
        }

        inline UserInput SFMLWindow::getPressedUserInputFromSfKeyboard(const sf::Event::KeyEvent& key)
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
            // Convert base to Pressed
            if (result.type == UserInputTypeKeyboardChar) {
                result.type = UserInputTypeKeyboardCharPressed;
            } else {
                result.type = UserInputTypeKeyboardSpecialPressed;
            }
            return result;
        }

        ///         +0
        ///   -1 joystick  +1
        ///         -0
        ///                                     +4
        ///                              -5  joystick +5
        ///                                     -4
        ///          7
        ///      6 d-pad 6
        ///          7
        ///
        inline UserInput SFMLWindow::processJoystickMove(const sf::Event& event)
        {
            UserInput input;

            input.data.joystickInput.joystickId = event.joystickMove.joystickId;
            // left joystick
            if (event.joystickMove.axis == 0) {
                input.type = UserInputTypeJoystickLeftMove;
                input.data.joystickInput.data.joystickPosition.x = event.joystickMove.position;
            } else if (event.joystickMove.axis == 1) {
                input.type = UserInputTypeJoystickLeftMove;
                input.data.joystickInput.data.joystickPosition.y = event.joystickMove.position;
            }
            // right joystick
            if (event.joystickMove.axis == 4) {
                input.type = UserInputTypeJoystickRightMove;
                input.data.joystickInput.data.joystickPosition.x = event.joystickMove.position;
            } else if (event.joystickMove.axis == 5) {
                input.type = UserInputTypeJoystickRightMove;
                input.data.joystickInput.data.joystickPosition.y = event.joystickMove.position;
            }
            // d-pad
            if (event.joystickMove.axis == 7) {
                input.type = UserInputTypeJoystickDPad;
                input.data.joystickInput.data.dpadPosition.y = event.joystickMove.position;
            } else if (event.joystickMove.axis == 6) {
                input.type = UserInputTypeJoystickDPad;
                input.data.joystickInput.data.dpadPosition.x = event.joystickMove.position;
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
        inline void SFMLWindow::processJoystickMoveWithSfJoystickInsteadOfStupidSfEventDeConStupide(unsigned int joystickId)
        {
            UserInput input;
            float axis = 0.0f;

            input.data.joystickInput.joystickId = joystickId;
            // Checking joystick threshold myself since sfml no work
            // left joystick
            axis = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Axis(0));
            if (axis != 0.0f && std::abs(axis) > this->_windowSpecs.joystickThreshold) {
                input.type = UserInputTypeJoystickLeftMove;
                input.data.joystickInput.data.joystickPosition.x = axis;
                this->_inputManager.addInput(input);
            }
            axis = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Axis(1));
            if (axis != 0.0f && std::abs(axis) > this->_windowSpecs.joystickThreshold) {
                input.type = UserInputTypeJoystickLeftMove;
                input.data.joystickInput.data.joystickPosition.y = axis;
                this->_inputManager.addInput(input);
            }
            // right joystick
            axis = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Axis(4));
            if (axis != 0.0f && std::abs(axis) > this->_windowSpecs.joystickThreshold) {
                input.type = UserInputTypeJoystickRightMove;
                input.data.joystickInput.data.joystickPosition.x = axis;
                this->_inputManager.addInput(input);
            }
            axis = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Axis(5));
            if (axis != 0.0f && std::abs(axis) > this->_windowSpecs.joystickThreshold) {
                input.type = UserInputTypeJoystickRightMove;
                input.data.joystickInput.data.joystickPosition.y = axis;
                this->_inputManager.addInput(input);
            }
            // d-pad
            axis = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Axis(7));
            if (axis != 0.0f && std::abs(axis) > this->_windowSpecs.joystickThreshold) {
                input.type = UserInputTypeJoystickDPad;
                input.data.joystickInput.data.dpadPosition.y = axis;
                this->_inputManager.addInput(input);
            }
            axis = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Axis(6));
            if (axis != 0.0f && std::abs(axis) > this->_windowSpecs.joystickThreshold) {
                input.type = UserInputTypeJoystickDPad;
                input.data.joystickInput.data.dpadPosition.x = axis;
                this->_inputManager.addInput(input);
            }
        }
        void SFMLWindow::removeJoystickFromVector(unsigned int joystickId)
        {
            std::vector<unsigned int>::const_iterator it = this->_joystickIds.begin();

            while (it != this->_joystickIds.end()) {
                if (*it == joystickId) {
                    this->_joystickIds.erase(it);
                    return;
                }
                it++;
            }
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
        void SFMLWindow::setShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
        {
            if (this->_shader.has_value() == false) {
                this->_shader.emplace();
            }
            try {
                bool result = false;

                if (vertexShaderPath != "" && fragmentShaderPath != "") {
                    result = this->_shader->loadFromFile(vertexShaderPath, fragmentShaderPath);
                } else if (vertexShaderPath != "") {
                    result = this->_shader->loadFromFile(vertexShaderPath, sf::Shader::Vertex);
                } else if (fragmentShaderPath != "") {
                    result = this->_shader->loadFromFile(fragmentShaderPath, sf::Shader::Fragment);
                }
                if (result == false) {
                    throw WindowException("Cannot load shaders from vertex file '" + vertexShaderPath + "' and fragment file '" + fragmentShaderPath + "'.");
                }
            } catch (std::exception& e) {
                std::string msg = e.what();

                throw WindowException("Error on setShader: " + msg);
            }
        }

    }  // namespace Graphics
}  // namespace Rengine
