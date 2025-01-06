//
#include <gtest/gtest.h>

#include "../../src/Graphics/WindowSpecs.hpp"


/* window.json be like:
{
    "title": "window",
    "resolution": {
        "width": 800,
        "height": 600
    },
    "framerateLimit": 60,
    "backgroundColor": {
        "r": 1,
        "g": 2,
        "b": 3
    },
    "iconPath": "assets/icon.png",
    "options": {
        "isFullscreen": false,
        "isResizable": true,
        "isBorderless": false,
        "enableVsync": true,
        "isCursorVisible": true
    }
} */

TEST(WindowSpecs, buildFromJson)
{
    Rengine::Graphics::WindowSpecs specs;

    specs.buildFromJson("Graphics/window.json");
    EXPECT_EQ(specs.title, "window");
    EXPECT_EQ(specs.resolution.x, 800);
    EXPECT_EQ(specs.resolution.y, 600);
    EXPECT_EQ(specs.framerateLimit, 60);
    EXPECT_EQ(specs.backgroundColor.x, 1);
    EXPECT_EQ(specs.backgroundColor.y, 2);
    EXPECT_EQ(specs.backgroundColor.z, 3);
    EXPECT_EQ(specs.iconPath, "assets/icon.png");
    // options
    EXPECT_EQ(specs.options.isFullscreen, false);
    EXPECT_EQ(specs.options.isResizable, true);
    EXPECT_EQ(specs.options.isBorderless, false);
    EXPECT_EQ(specs.options.enableVsync, true);
    EXPECT_EQ(specs.options.isCursorVisible, true);
}
