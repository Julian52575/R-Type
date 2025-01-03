//
#include <gtest/gtest.h>
#include <cstdint>
#include <math.h>

#include "../../src/Config/EntityConfig.hpp"
/* Config/entity.json be like
{
    "entity": {
        "sprite": {
            "assetPath": "assets/images/red.png",
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
        },
        "hitbox": {
            "size": {
                "x": 13, "y": 14
            },
            "offset": {
                "x": 15, "y": 16
            }
        },
        "stats": {
            "hp": 17,
            "attack": 18,
            "defense": 19,
            "minSpeed": 20,
            "acceleration": 21.21,
            "maxSpeed": 22,
            "enemyColisionDamage": 23
        },
        "characteristics": {
            "enemyDamageOnColision": true,
            "damagedOnColision": true,
            "persistsOffScreen": false
        },
        "attacks": {
            "shoot1": "Config/buff.json",
            "shoot2": "Config/missiles.json"
        }
    }
}*/
TEST(EntityConfig, EntityConfigSprite)
{
    auto con = RType::Config::EntityConfig("Config/entity.json");
    const RType::Config::ImageConfigData &sp = con.getSprite().getConfig();
    // Sprite
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
