//
#include "gtest/gtest.h"
#include <functional>
#include <gtest/gtest.h>
#include <type_traits>

#include "../../src/Graphics/UserInputManager.hpp"

TEST(UserInput, operatorEqual)
{
    Rengine::Graphics::UserInput escape = {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputKeyboardSpecialESCAPE};
    Rengine::Graphics::UserInput space = {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputKeyboardSpecialSPACE};
    Rengine::Graphics::UserInput a = {Rengine::Graphics::UserInputType::UserInputTypeKeyboardChar, 'a'};
    Rengine::Graphics::UserInput b = {Rengine::Graphics::UserInputType::UserInputTypeKeyboardChar, 'b'};
    Rengine::Graphics::UserInput close = {Rengine::Graphics::UserInputTypeWindowClosed, {0}};
    Rengine::Graphics::vector2D<float> joystick2d = {10.1, 20.2};
    Rengine::Graphics::UserInput joystick = {Rengine::Graphics::UserInputTypeJoystickRightMove};
    Rengine::Graphics::vector2D<float> joystick2d2 = {99.9, 99.9};
    Rengine::Graphics::UserInput joystick2 = {Rengine::Graphics::UserInputTypeJoystickRightMove};

    joystick.data.joystickPosition = joystick2d;
    joystick2.data.joystickPosition = joystick2d2;

    // escape
    EXPECT_EQ(escape, escape);
    EXPECT_NE(escape, space);
    EXPECT_NE(escape, a);
    EXPECT_NE(escape, b);
    EXPECT_NE(escape, close);
    EXPECT_NE(escape, joystick);
    // space
    EXPECT_EQ(space, space);
    EXPECT_NE(space, a);
    EXPECT_NE(space, b);
    EXPECT_NE(space, close);
    EXPECT_NE(space, joystick);
    // a
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
    EXPECT_NE(a, close);
    EXPECT_NE(a, joystick);
    // b
    EXPECT_EQ(b, b);
    EXPECT_NE(b, close);
    EXPECT_NE(b, joystick);
    // close
    EXPECT_EQ(close, close);
    EXPECT_NE(close, joystick);
    // joystick
    EXPECT_EQ(joystick, joystick);
    EXPECT_EQ(joystick, joystick2);
}
TEST(UserInput, receivedInput)
{
    Rengine::Graphics::UserInputManager manager;
    Rengine::Graphics::UserInput close = {Rengine::Graphics::UserInputTypeWindowClosed};
    Rengine::Graphics::UserInput a = {Rengine::Graphics::UserInputTypeKeyboardChar, 'a'};

    manager.addInput(close);
    std::optional<std::reference_wrapper<const Rengine::Graphics::UserInput>> result = manager.receivedInput(close);

    EXPECT_TRUE(result.has_value());
    std::optional<std::reference_wrapper<const Rengine::Graphics::UserInput>> result2 = manager.receivedInput(a);

    EXPECT_FALSE(result2.has_value());
}
