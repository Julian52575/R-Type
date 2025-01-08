//
#include <gtest/gtest.h>
#include <cstdint>
#include <math.h>
#include <rengine/src/Graphics/SpriteSpecs.hpp>

#include "../../src/Config/EntityConfig.hpp"
#include "../../src/Config//EntityConfigResolver.hpp"
/* Config/entity.json be like
{
    "entity": {
        "sprite": assets/images/red.png",
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
    const Rengine::Graphics::SpriteSpecs &sp = con.getSprite().getConfig();
    // Sprite
    Rengine::Graphics::SpriteSpecs expectedSprite;

    expectedSprite.texturePath = "assets/images/red.png";
    expectedSprite.originOffset = {1, 2};
    expectedSprite.textureScale = {3.3, 4.4};
    expectedSprite.animation->frameRectXY = {5, 6};
    expectedSprite.animation->frameRectWidthHeight = {7, 8};
    expectedSprite.animation->frameCount = 9;
    expectedSprite.animation->frameDuration = 10.10;
    expectedSprite.animation->frameDisplacement = {11, 12};
    ASSERT_EQ(sp.texturePath, expectedSprite.texturePath);
    ASSERT_EQ(sp.originOffset, expectedSprite.originOffset);
    ASSERT_EQ(sp.textureScale, expectedSprite.textureScale);
    ASSERT_EQ(sp.animation->frameRectXY, expectedSprite.animation->frameRectXY);
    ASSERT_EQ(sp.animation->frameRectWidthHeight, expectedSprite.animation->frameRectWidthHeight);
    ASSERT_EQ(sp.animation->frameCount, expectedSprite.animation->frameCount);
    ASSERT_EQ(sp.animation->frameDuration, expectedSprite.animation->frameDuration);
    ASSERT_EQ(sp.animation->frameDisplacement, expectedSprite.animation->frameDisplacement);
}
TEST(EntityConfig, EntityConfigCharacteristics)
{
    auto con = RType::Config::EntityConfig("Config/entity.json");
    const RType::Config::EntityConfigCharacteristics& chara = con.getCharacteristics();

    ASSERT_FALSE(chara.persistsOffScreen);
    ASSERT_TRUE(chara.damagedOnColision);
    ASSERT_TRUE(chara.enemyDamageOnColision);
}
TEST(EntityConfig, EntityConfigHitbox)
{
    auto con = RType::Config::EntityConfig("Config/entity.json");
    const RType::Config::EntityConfigHitbox& hitbox = con.getHitbox();

    ASSERT_EQ(hitbox.size.first, 13);
    ASSERT_EQ(hitbox.size.second, 14);
    ASSERT_EQ(hitbox.offsetFromSpriteOrigin.first, 15);
    ASSERT_EQ(hitbox.offsetFromSpriteOrigin.second, 16);
}
TEST(EntityConfig, EntityConfigStats)
{
    auto con = RType::Config::EntityConfig("Config/entity.json");
    const RType::Config::EntityConfigStats& stats = con.getStats();

    ASSERT_EQ(stats.hp, 17);
    ASSERT_EQ(stats.attack, 18);
    ASSERT_EQ(stats.defense, 19);
    ASSERT_EQ(stats.minSpeed, 20);
    ASSERT_EQ(stats.acceleration, 21.21f);
    ASSERT_EQ(stats.maxSpeed, 22);
    ASSERT_EQ(stats.enemyColisionDamage, 23);
}
TEST(EntityConfig, EntityConfigAttack)
{
    auto con = RType::Config::EntityConfig("Config/entity.json");
    const std::optional<RType::Config::AttackConfig>& shoot1 = con.getAttack(1);
    const std::optional<RType::Config::AttackConfig>& shoot2 = con.getAttack(2);
    const std::optional<RType::Config::AttackConfig>& shoot3 = con.getAttack(3);

    //shoot1
    ASSERT_TRUE(shoot1.has_value());
    ASSERT_TRUE(shoot1->getType() == RType::Config::AttackType::AttackTypeBuffs);
    ASSERT_EQ(shoot1->getCooldown(), 1.5);
    //shoot2
    ASSERT_TRUE(shoot2.has_value());
    ASSERT_TRUE(shoot2->getType() == RType::Config::AttackType::AttackTypeMissiles);
    ASSERT_EQ(shoot2->getCooldown(), 3.0);
    //shoot3
    ASSERT_FALSE(shoot3.has_value());
}


/* buff.json
{
    "attack": {
        "type": "buff",
        "cooldown": 1.5,
        "buffs": [
            {
                "type": "attack",
                "value": 1.5,
                "duration": 3.0
            },
            {
                "type": "speed",
                "value": 2.0,
                "duration": 1.0
            },
            {
                "type": "hpHeal",
                "value": 100
            },
            {
                "type": "hpHealTime",
                "value": 5,
                "duration": 1.5
            },
            {
                "type": "defense",
                "value": 1.5,
                "duration": 1.5
            }
        ]
    }
}*/
/* missiles.json
{
    "attack": {
        "type": "missiles",
        "cooldown": 3.0,
        "missiles": [
            {
                "json": "missile.json",
                "offset": {
                    "x": 1.1, "y": 1.2
                },
                "velocity": {
                    "x": 1.3, "y": 1.4
                }
            },
            {
                "json": "missileUp.json",
                "offset": {
                    "x": 2.1, "y": 2.2
                },
                "velocity": {
                    "x": 2.3, "y": 2.4
                }
            },
            {
                "json": "missileDown.json",
                "offset": {
                    "x": -3.1, "y": -3.2
                },
                "velocity": {
                    "x": -3.3, "y": -3.4
                }
            }
        ]
    }
}*/
