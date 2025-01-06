#include <cstddef>
#include <memory>
#include <vector>

#include "UserInputManager.hpp"
#include "Vector.hpp"

namespace Rengine {
    namespace Graphics {

        UserInputManager::UserInputManager(uint16_t maxInputPerFrame)
        {
            this->_maxInputPerFrame = maxInputPerFrame;
            this->_inputVector.reserve(maxInputPerFrame);
        }
        UserInputManager::const_iterator UserInputManager::begin(void) const
        {
            return this->_inputVector.begin();
        }
        UserInputManager::const_iterator UserInputManager::end(void) const
        {
            return this->_inputVector.end();
        }

        void UserInputManager::addInput(const Rengine::Graphics::UserInput& input)
        {
            if (this->_inputVector.size() >= this->_maxInputPerFrame) {
                return;
            }
            this->_inputVector.push_back(input);
        }
        void UserInputManager::clear(void)
        {
            this->_inputVector.clear();
        }
        UserInputManager::size_type UserInputManager::size(void) const noexcept
        {
            return this->_inputVector.size();
        }


        std::ostream& operator<<(std::ostream& os, const UserInputType& inputType)
        {
            std::string str;

            switch (inputType) {
                case UserInputTypeNA:
                    str = "UserInputTypeNA";
                    break;
                case UserInputTypeKeyboardChar:
                    str = "UserInputTypeKeyboardChar";
                    break;
                case UserInputTypeKeyboardSpecial:
                    str = "UserInputTypeKeyboardSpecial";
                    break;
                case UserInputTypeMouseLeftClick:
                    str = "UserInputTypeMouseLeftClick";
                    break;
                case UserInputTypeMouseRightClick:
                    str = "UserInputTypeMouseRightClick";
                    break;
                case UserInputTypeJoystickConnected:
                    str = "UserInputTypeJoystickConnected";
                    break;
                case UserInputTypeJoystickDisconnected:
                    str = "UserInputTypeJoystickDisconnected";
                    break;
                case UserInputTypeJoystickLeftMove:
                    str = "UserInputTypeJoystickLeftMove";
                    break;
                case UserInputTypeJoystickLeftPressed:
                    str = "UserInputTypeJoystickLeftPressed";
                    break;
                case UserInputTypeJoystickRightMove:
                    str = "UserInputTypeJoystickRightMove";
                    break;
                case UserInputTypeJoystickRightPressed:
                    str = "UserInputTypeJoystickRightPressed";
                    break;
                case UserInputTypeWindowClosed:
                    str = "UserInputTypeWindowClosed";
                    break;
                case UserInputTypeJoystickButton:
                    str = "UserInputTypeJoystickButton";
                    break;
                default:
                    str = "Unknow";
                    break;
            }
            os << "UserInputType::" << str << std::endl;
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const UserInput& input)
        {
            os << input.type;
            return os;
        }

    }  // namespace Graphic
}  // namespace Rengine
