//
#include <cstddef>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <rengine/Rengine.hpp>


#include "../Network/EntityAction.hpp"
#include "Action.hpp"

namespace RType {
    namespace Components {

        Action::Action(ActionSource source, const std::string& luaScript)
        {
            this->_actionSource = source;
            switch (source) {
                case ActionSourceLua:
                    std::cout << "RType::Component::Action: Warning: Lua not inplemented." << luaScript << std::endl;
                    break;

                case ActionSourceUserInput:
                    this->buildBindVector();
                    break;

                default:
                    throw std::runtime_error("RType::Component::Action: Unknow ActionSource.");
            }  // switch source
            this->_actionVector.reserve(5);
        }
        void Action::processUserInput(const Rengine::Graphics::UserInput& input)
        {
            // Check if source is lua script
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Trying to set user input on lua source.");
            }
            Network::EntityAction newAction;

            // Parse input vector
            auto it = this->_inputNetworkBindVector.begin();

            while (it != this->_inputNetworkBindVector.end()) {
                bool dataComparaison = false;

                if (it->first.type == input.type) {
                    // Check KeyboardSpecial
                    if (it->first.type == Rengine::Graphics::UserInputTypeKeyboardSpecial) {
                        dataComparaison = (it->first.data.keyboardSpecial == input.data.keyboardSpecial);
                    } else if (it->first.type == Rengine::Graphics::UserInputTypeKeyboardChar) {  // Check KeyboardChar
                        dataComparaison = (it->first.data.keyboardChar == input.data.keyboardChar);
                    }
                    if (dataComparaison == true) {
                        newAction.type = it->second;
                        break;
                    }
                }
                it++;
            }  // while it != end
            // No match
            if (it == this->_inputNetworkBindVector.end()) {
                return;
            }

            // Handle movement
            if (newAction.type == Network::EntityActionTypeMove) {
                float moveX = 0.0f;
                float moveY = 0.0f;
                // Handle keyboard arrows
                if (input.type == Rengine::Graphics::UserInputTypeKeyboardSpecial) {
                    switch (input.data.keyboardSpecial) {
                        // Down arrow
                        case Rengine::Graphics::UserInputTypeKeyboardSpecialArrowDOWN:
                            moveY = 80;
                            break;
                        // Up arrow
                        case Rengine::Graphics::UserInputTypeKeyboardSpecialArrowUP:
                            moveY = -80;
                            break;
                        // Left arrow
                        case Rengine::Graphics::UserInputTypeKeyboardSpecialArrowLEFT:
                            moveX = -80;
                            break;
                        // Right arrow
                        case Rengine::Graphics::UserInputTypeKeyboardSpecialArrowRIGHT:
                            moveX = 80;
                            break;
                        // Error handling
                        default:
                            break;
                    }  // switch input.data.keyboardSpecial
                } else if (input.type == Rengine::Graphics::UserInputTypeJoystickLeftMove
                        || input.type == Rengine::Graphics::UserInputTypeJoystickRightMove) {
                    moveX = input.data.joystickPosition.x;
                    moveY = input.data.joystickPosition.y;
                }
                newAction.data.moveVelocity.x = moveX;
                newAction.data.moveVelocity.y = moveY;
            }  // if newAction.type == Network::EntityActionTypeMove
            this->_actionVector.push_back(newAction);
        }
        void Action::buildBindVector(void)
        {
            this->_inputNetworkBindVector = {
                {
                    {Rengine::Graphics::UserInputTypeKeyboardChar, ' '},
                    Network::EntityActionType::EntityActionTypeShoot1
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputTypeKeyboardSpecialSHIFT},
                    Network::EntityActionType::EntityActionTypeShoot2
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputTypeKeyboardSpecialTAB},
                    Network::EntityActionType::EntityActionTypeShoot3
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputTypeKeyboardSpecialArrowUP},
                    Network::EntityActionType::EntityActionTypeMove
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputTypeKeyboardSpecialArrowDOWN},
                    Network::EntityActionType::EntityActionTypeMove
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputTypeKeyboardSpecialArrowLEFT},
                    Network::EntityActionType::EntityActionTypeMove
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputTypeKeyboardSpecialArrowRIGHT},
                    Network::EntityActionType::EntityActionTypeMove
                }
            };  // this->_inputNetworkBindVector
        }
        Action::const_iterator Action::begin(void) const
        {
            return this->_actionVector.begin();
        }
        Action::const_iterator Action::end(void) const
        {
            return this->_actionVector.end();
        }
        void Action::clear(void)
        {
            this->_actionVector.clear();
        }
        Action::size_type Action::size(void) const noexcept
        {
            return this->_actionVector.size();
        }
        void Action::changeInput(Rengine::Graphics::UserInput newInput, Network::EntityActionType resultingAction)
        {
            // Check if source is lua script
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Trying to set user input on lua source.");
            }
        }
        const Rengine::Graphics::UserInput Action::getNeededInput(Network::EntityActionType act) const
        {
            auto it = this->_inputNetworkBindVector.begin();

            while (it != this->_inputNetworkBindVector.end()) {
                if (it->second == act) {
                    return it->first;
                }
                it++;
            }
            return {Rengine::Graphics::UserInputTypeNA, {0}};
        }
    }  // namespace Components
}  // namespace RType
