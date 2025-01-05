//
#ifndef _SRC_GRAPHICS_INPUTMANAGER_HPP_
#define _SRC_GRAPHICS_INPUTMANAGER_HPP_
#include <cstddef>
#include <memory>
#include <vector>
#include <iostream>

#include "Vector.hpp"

namespace Rengine {
    namespace Graphics {
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class UserInputType
        * @brief An enum for the different type of user input.
        */
        enum UserInputType {
            UserInputTypeNA,
            UserInputTypeKeyboardChar,
            UserInputTypeKeyboardSpecial,
            UserInputTypeMouseLeftClick,
            UserInputTypeMouseRightClick,
            UserInputTypeJoystickConnected,
            UserInputTypeJoystickDisconnected,
            UserInputTypeJoystickLeftMove,
            UserInputTypeJoystickLeftPressed,
            UserInputTypeJoystickRightMove,
            UserInputTypeJoystickRightPressed,
            UserInputTypeJoystickButton,
            UserInputTypeWindowClosed
        };
        std::ostream& operator<<(std::ostream& os, const UserInputType& inputType);

        enum InputTypeKeyboardSpecial {
            UserInputTypeKeyboardSpecialNA,
            UserInputTypeKeyboardSpecialESCAPE,
            UserInputTypeKeyboardSpecialTAB,
            UserInputTypeKeyboardSpecialSHIFT,
            UserInputTypeKeyboardSpecialArrowUP,
            UserInputTypeKeyboardSpecialArrowDOWN,
            UserInputTypeKeyboardSpecialArrowLEFT,
            UserInputTypeKeyboardSpecialArrowRIGHT
        };

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class UserInputData
        * @brief A union containing the data for the different input type.
        */
        union UserInputData {
            char keyboardChar;
            enum InputTypeKeyboardSpecial keyboardSpecial;
            Rengine::Graphics::vector2D<float> mousePosition;
            /**
            * @brief the position of the joystick from -100 to +100
            */
            Rengine::Graphics::vector2D<float> joystickPosition;
            unsigned int joystickButton;
        };
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class UserInput
        * @brief A structure containing the data for a user input.
        */
        struct UserInput {
            UserInputType type = UserInputTypeNA;
            UserInputData data = {0};
        };
        std::ostream& operator<<(std::ostream& os, const UserInput& input);

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class UserInputManager
        * @brief This class manages the user input
        */
        class UserInputManager {
            public:
                using container_t = std::vector<Rengine::Graphics::UserInput>;
                using size_type = typename container_t :: size_type;
                using iterator = typename container_t :: iterator;
                using const_iterator = typename container_t :: const_iterator;
                UserInputManager(std::size_t maxInputPerFrame = 100);
                ~UserInputManager(void) = default;

            public:
                /**
                * @fn begin
                * @return A const iterator of UserInput.
                * @brief Get a const iterator from the UserInput vector.
                */
                const_iterator begin(void) const;
                const_iterator end(void) const;
                /**
                * @fn clear
                * @brief Remove all the user input from the vector.
                */
                void clear(void);
                /**
                * @fn size
                * @return The number of input since the last reset.
                * @brief Get the number of input.
                */
                size_type size(void) const noexcept;


            public:
                /**
                * @fn begin
                * @param Rengine::Graphics::UserInput the new user input.
                * @brief Add an input to the input vector.
                * Note: No input will be added once the vector has the max number of input set at creation.
                */
                void addInput(const Rengine::Graphics::UserInput& input);

            private:
                container_t _inputVector;
                std::size_t _maxInputPerFrame;
        };


    }  // namespace Graphic
}  // namespace Rengine
#endif
