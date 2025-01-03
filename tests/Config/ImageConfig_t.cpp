//
#include <gtest/gtest.h>

#include "../../src/Config/ImageConfig.hpp"

/* image.json be like: {
    "image": {
        "texturePath": "assets/images/red.png",
        "origin": {
            "x": 1,
            "y": 2
        },
        "scale": {
            "x": 3.3,
            "y": 4.4
        },
        "animations": {
            "frameRect": {
                "x": 5, "y": 6, "width": 7, "height": 8
            },
            "frameCount": 9,
            "frameDuration": 10.10,
            "frameDisplacement": {
                "x": 11, "y": 12
            }
        }
    }
}*/
TEST(ImageConfig, ImageConfig)
{
    RType::Config::ImageConfig im("Config/image.json");
    const RType::Config::ImageConfigData &sp = im.getConfig();
    RType::Config::ImageConfigData expectedSprite;

    expectedSprite.texturePath = "assets/images/red.png";
    expectedSprite.originOffset = {1, 2};
    expectedSprite.textureScale = {3.3, 4.4};
    expectedSprite.animation.frameRectXY = {5, 6};
    expectedSprite.animation.frameRectWidthHeight = {7, 8};
    expectedSprite.animation.frameCount = 9;
    expectedSprite.animation.frameDuration = 10.10;
    expectedSprite.animation.frameDisplacement = {11, 12};
    ASSERT_EQ(sp.texturePath, expectedSprite.texturePath);
    ASSERT_EQ(sp.originOffset, expectedSprite.originOffset);
    ASSERT_EQ(sp.textureScale, expectedSprite.textureScale);
    ASSERT_EQ(sp.animation.frameRectXY, expectedSprite.animation.frameRectXY);
    ASSERT_EQ(sp.animation.frameRectWidthHeight, expectedSprite.animation.frameRectWidthHeight);
    ASSERT_EQ(sp.animation.frameCount, expectedSprite.animation.frameCount);
    ASSERT_EQ(sp.animation.frameDuration, expectedSprite.animation.frameDuration);
    ASSERT_EQ(sp.animation.frameDisplacement, expectedSprite.animation.frameDisplacement);
}
