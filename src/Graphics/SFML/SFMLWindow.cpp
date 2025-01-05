//
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <exception>
#include <memory>
#include <iostream>

#include "SFMLWindow.hpp"
#include "SFMLSprite.hpp"

namespace Rengine {
    namespace Graphics {

        SFMLWindow::SFMLWindow(const sf::VideoMode& videoMode, const std::string& windowTitle) : _renderWindow(videoMode, windowTitle)
        {
            UserInput a;
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
                // Special
                {sf::Keyboard::Escape, {UserInputTypeKeyboardSpecial, UserInputTypeKeyboardSpecialESCAPE}},
                {sf::Keyboard::Tab, {UserInputTypeKeyboardSpecial, UserInputTypeKeyboardSpecialTAB}},
                {sf::Keyboard::LShift, {UserInputTypeKeyboardSpecial, UserInputTypeKeyboardSpecialSHIFT}},
                {sf::Keyboard::RShift, {UserInputTypeKeyboardSpecial, UserInputTypeKeyboardSpecialSHIFT}}
            };
        }
        void SFMLWindow::render(void)
        {
            this->_renderWindow.display();
            this->_renderWindow.clear();
        }
        void SFMLWindow::addSpriteToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite, const Rengine::Graphics::vector2D<float>& position)
        {
            try {
                Rengine::Graphics::SFMLSprite& sfmlSprite = (SFMLSprite&) *sprite;
                sf::Sprite& sfSprite = sfmlSprite.getSfSprite();
                sf::Vector2f posVector = {position.x, position.y};

                sfSprite.setPosition(posVector);
                this->_renderWindow.draw(sfSprite);
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
                        newInput = getUserInputFromSfKeyboard(event.key);
                        if (newInput.type == UserInputType::UserInputTypeNA) {
                            continue;
                        }
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

                    default:
                        continue;
                }  // switch(event.type)
                this->_inputManager.addInput(newInput);
            }  // while (this->_renderWindow.pollEvent(event)
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

    }  // namespace Rengine
}  // namespace Graphics
