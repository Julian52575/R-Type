#include "KeyBoard.hpp"

System::KeyBoardInput::KeyBoardInput() {}

System::KeyBoardInput::~KeyBoardInput() {}

void System::KeyBoardInput::update(sparse_array<Controllable>& keyBoards, sparse_array<Velocity>& velocities) {
    for (size_t i = 0; i < keyBoards.size() && i < velocities.size(); i++) {
        if (keyBoards[i].has_value() && velocities[i].has_value()) {
            if (sf::Keyboard::isKeyPressed(keyBoards[i].value().up))
                velocities[i].value().y = -(velocities[i].value().getBase().second);
            else if (sf::Keyboard::isKeyPressed(keyBoards[i].value().down))
                velocities[i].value().y = velocities[i].value().getBase().second;
            else 
                velocities[i].value().y = 0;

            if (sf::Keyboard::isKeyPressed(keyBoards[i].value().left))
                velocities[i].value().x = -(velocities[i].value().getBase().first);
            else if (sf::Keyboard::isKeyPressed(keyBoards[i].value().right))
                velocities[i].value().x = velocities[i].value().getBase().first;
            else
                velocities[i].value().x = 0;
        }
    }
    for (size_t i = 0; i < keyBoards.size() && i < velocities.size(); i++) {
        if (keyBoards[i].has_value() && velocities[i].has_value()) {
            float joystickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
            float joystickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

            if (std::abs(joystickY) > 15) { // Deadzone threshold
                velocities[i].value().y = joystickY / 100 * velocities[i].value().getBase().second;
            } else {
                // velocities[i].value().y = 0;
            }

            if (std::abs(joystickX) > 15) { // Deadzone threshold
                velocities[i].value().x = joystickX / 100 * velocities[i].value().getBase().first;
            } else {
                // velocities[i].value().x = 0;
            }
        }
    }
}