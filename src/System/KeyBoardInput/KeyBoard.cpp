#include "./KeyBoard.hpp"

System::KeyBoardInput::KeyBoardInput() {}

System::KeyBoardInput::~KeyBoardInput() {}

void System::KeyBoardInput::update(sparse_array<Controllable>& keyBoards, sparse_array<Velocity>& velocities) {
    for (uint32_t i = 0; i < keyBoards.size() && i < velocities.size(); i++) {
        if (keyBoards[i].has_value() && velocities[i].has_value()) {
            velocities[i].value().x = 0;
            velocities[i].value().y = 0;
            if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) != 0 || sf::Joystick::getAxisPosition(0, sf::Joystick::X) != 0) {
                float joystickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
                float joystickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
                if (std::abs(joystickY) > 15)
                    velocities[i].value().y = joystickY / 100 * velocities[i].value().getBase().second;
                if (std::abs(joystickX) > 15)
                    velocities[i].value().x = joystickX / 100 * velocities[i].value().getBase().first;
                if (velocities[i].value().x != 0 || velocities[i].value().y != 0)
                    continue;
            }
            if (sf::Keyboard::isKeyPressed(keyBoards[i].value().up))
                velocities[i].value().y = -(velocities[i].value().getBase().second);
            else if (sf::Keyboard::isKeyPressed(keyBoards[i].value().down))
                velocities[i].value().y = velocities[i].value().getBase().second;

            if (sf::Keyboard::isKeyPressed(keyBoards[i].value().left))
                velocities[i].value().x = -(velocities[i].value().getBase().first);
            else if (sf::Keyboard::isKeyPressed(keyBoards[i].value().right))
                velocities[i].value().x = velocities[i].value().getBase().first;
        }
    }
}

void System::KeyBoardInput::shoot(sparse_array<Controllable>& keyBoards, sparse_array<Position>& positions, sparse_array<Attack>& attacks,float deltatime, std::function<void(float, float)> makePlayerAttack){
    for (uint32_t i = 0; i < keyBoards.size() && i < positions.size() && i < attacks.size(); i++) {
        if (keyBoards[i].has_value() && positions[i].has_value() && attacks[i].has_value()) {
            attacks[i].value().update(deltatime);
            if (sf::Keyboard::isKeyPressed(keyBoards[i].value().shoot) ||
                sf::Joystick::isButtonPressed(0, 1) || // O button for PS5 controller
                sf::Joystick::isButtonPressed(0, 0)) { // A button for Nintendo controller
                float playerX = positions[i].value().x;
                float playerY = positions[i].value().y;
                if (attacks[i].value().canAttack()){
                    makePlayerAttack(playerX, playerY);
                    attacks[i].value().reset();
                }
            }
        }
    }
}
