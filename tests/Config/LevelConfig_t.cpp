//
#include <gtest/gtest.h>
#include <regex.h>
#include <rengine/src/Graphics/SpriteSpecs.hpp>
#include <vector>

#include "../../src/Config/LevelConfig.hpp"
/*level.json = {
    "level": {
        "scenes": [
            {
                "background": [
                    "Config/image.json"
                ],
                "scrollingSpeed": 15.15,
                "enemies": [
                    {
                        "json": "Config/entity.json",
                        "x": 2000,
                        "y": 540
                    },
                    {
                        "json": "Config/entity.json",
                        "x": 2150,
                        "y": 1000
                    }
                ],
                "endCondition": "time",
                "endConditionTime": 10.10
            },
            {
                "background": [
                    "Config/image.json",
                    "Config/image.json"
                ],
                "scrollingSpeed": 30.30,
                "enemies": [
                    {
                        "json": "Config/entity.json",
                        "x": 2300,
                        "y": 540
                    },
                    {
                        "json": "Config/entity.json",
                        "x": 2600,
                        "y": 1000
                    },
                    {
                        "json": "Config/entity.json",
                        "x": 2900,
                        "y": 100,
                        "boss": true
                    }
                ],
                "endCondition": "bossDefeat"
            }
        ]
    }
}
*/

/* image.json be like {
   "image":{
      "texturePath":"assets/images/red.png",
      "origin":{
         "x":1,
         "y":2
      },
      "scale":{
         "x":3.3,
         "y":4.4
      },
      "animations":{
         "frameRect":{
            "x":5,
            "y":6,
            "width":7,
            "height":8
         },
         "frameCount":9,
         "frameDuration":10.10,
         "frameDisplacement": {
            "x":11,
            "y":12
         }
      }
   }
}
*/

/* entity.json be like {
    "entity": {
        "sprite": "Config/image.json",
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
}
*/

static inline Rengine::Graphics::SpriteSpecs getExpectedImageConfigData(void)
{
    Rengine::Graphics::SpriteSpecs expectedImage;

    expectedImage.texturePath = "assets/images/red.png";
    expectedImage.originOffset = {1, 2};
    expectedImage.textureScale = {3.3f, 4.4f};
    expectedImage.animation.frameRectXY = {5, 6};
    expectedImage.animation.frameRectWidthHeight = {7, 8};
    expectedImage.animation.frameCount = 9;
    expectedImage.animation.frameDuration = 10.10f;
    expectedImage.animation.frameDisplacement = {11, 12};
    return expectedImage;
}
static inline RType::Config::EntityConfig getExpectedEntityConfig(void)
{
    RType::Config::EntityConfig config("Config/entity.json");

    return config;
}
static inline void compareImageConfigData(const Rengine::Graphics::SpriteSpecs& sp)
{
    auto expectedSprite = getExpectedImageConfigData();

    ASSERT_EQ(sp.texturePath, expectedSprite.texturePath);
    ASSERT_EQ(sp.originOffset, expectedSprite.originOffset);
    ASSERT_EQ(sp.textureScale, expectedSprite.textureScale);
    ASSERT_EQ(sp.animation.frameRectXY, expectedSprite.animation.frameRectXY);
    ASSERT_EQ(sp.animation.frameRectWidthHeight, expectedSprite.animation.frameRectWidthHeight);
    ASSERT_EQ(sp.animation.frameCount, expectedSprite.animation.frameCount);
    ASSERT_EQ(sp.animation.frameDuration, expectedSprite.animation.frameDuration);
    ASSERT_EQ(sp.animation.frameDisplacement, expectedSprite.animation.frameDisplacement);
}
static inline void compareEntityConfig(const RType::Config::EntityConfig& con)
{
    auto expectedEntity = getExpectedEntityConfig();
    // Stats
    const RType::Config::EntityConfigStats& stats = con.getStats();
    auto exStats = expectedEntity.getStats();

    ASSERT_EQ(stats.hp, exStats.hp);
    ASSERT_EQ(stats.attack, exStats.attack);
    ASSERT_EQ(stats.defense, exStats.defense);
    ASSERT_EQ(stats.minSpeed, exStats.minSpeed);
    ASSERT_EQ(stats.acceleration, exStats.acceleration);
    ASSERT_EQ(stats.maxSpeed, exStats.maxSpeed);
    ASSERT_EQ(stats.enemyColisionDamage, exStats.enemyColisionDamage);
    // Hitbox
    const RType::Config::EntityConfigHitbox& hitbox = con.getHitbox();
    auto exHitbox = expectedEntity.getHitbox();

    ASSERT_EQ(hitbox.size.first, exHitbox.size.first);
    ASSERT_EQ(hitbox.size.second, exHitbox.size.second);
    ASSERT_EQ(hitbox.offsetFromSpriteOrigin.first, exHitbox.offsetFromSpriteOrigin.first);
    ASSERT_EQ(hitbox.offsetFromSpriteOrigin.second, exHitbox.offsetFromSpriteOrigin.second);
    // Sprite
    compareImageConfigData(con.getSprite().getConfig());
    // Characteristics
    const RType::Config::EntityConfigCharacteristics& chara = con.getCharacteristics();
    auto exChara = expectedEntity.getCharacteristics();

    ASSERT_EQ(chara.damagedOnColision, exChara.damagedOnColision);
    ASSERT_EQ(chara.enemyDamageOnColision, exChara.enemyDamageOnColision);
    ASSERT_EQ(chara.persistsOffScreen, exChara.persistsOffScreen);
    // Attack
    ASSERT_EQ(con.getAttack(0).has_value(), expectedEntity.getAttack(0).has_value());
    ASSERT_EQ(con.getAttack(1).has_value(), expectedEntity.getAttack(1).has_value());
    ASSERT_EQ(con.getAttack(2).has_value(), expectedEntity.getAttack(2).has_value());
}

TEST(LevelConfig, Scenes)
{
    RType::Config::LevelConfig lvl("Config/level.json");
    const std::vector<RType::Config::SceneConfig>& scenes = lvl.getScenes();

    ASSERT_EQ(scenes.size(), 2);
    /* Scene1 */
    const RType::Config::SceneConfig& scene1 = scenes[0];

    ASSERT_EQ(scene1.scrollingSpeed, 15.15f);
    ASSERT_EQ(scene1.endCondition, RType::Config::SceneEndCondition::SceneEndConditionTime);
    ASSERT_EQ(scene1.endConditionData.time, 10.10f);
    ASSERT_EQ(scene1.backgroundImages.size(), 1);
    // Background
    compareImageConfigData(scene1.backgroundImages[0].getConfig());
    // Enemies
    ASSERT_EQ(scene1.enemies.size(), 2);
    const RType::Config::SceneEntityConfig& scene1Entity1 = scene1.enemies[0];
    const RType::Config::SceneEntityConfig& scene1Entity2 = scene1.enemies[1];

    // scene1Entity1
    compareEntityConfig(scene1Entity1.entityConfig);
    ASSERT_EQ(scene1Entity1.xSpawn, 2000);
    ASSERT_EQ(scene1Entity1.ySpawn, 540);
    ASSERT_FALSE(scene1Entity1.isBoss);
    // scene1Entity2
    compareEntityConfig(scene1Entity2.entityConfig);
    ASSERT_EQ(scene1Entity2.xSpawn, 2150);
    ASSERT_EQ(scene1Entity2.ySpawn, 1000);
    ASSERT_FALSE(scene1Entity2.isBoss);

    /* Scene2 */
    const RType::Config::SceneConfig& scene2 = scenes[1];

    ASSERT_EQ(scene2.endCondition, RType::Config::SceneEndCondition::SceneEndConditionBossDefeat);
    ASSERT_EQ(scene2.scrollingSpeed, 30.30f);
    ASSERT_EQ(scene2.backgroundImages.size(), 2);
    compareImageConfigData(scene2.backgroundImages[0].getConfig());
    compareImageConfigData(scene2.backgroundImages[1].getConfig());
    ASSERT_EQ(scene2.enemies.size(), 3);
    const RType::Config::SceneEntityConfig& scene2Entity1 = scene2.enemies[0];
    const RType::Config::SceneEntityConfig& scene2Entity2 = scene2.enemies[1];
    const RType::Config::SceneEntityConfig& scene2Entity3 = scene2.enemies[2];

    // scene2Entity1
    ASSERT_EQ(scene2Entity1.xSpawn, 2300);
    ASSERT_EQ(scene2Entity1.ySpawn, 540);
    ASSERT_FALSE(scene2Entity1.isBoss);
    compareEntityConfig(scene2Entity1.entityConfig);
    // scene2Entity2
    ASSERT_EQ(scene2Entity2.xSpawn, 2600);
    ASSERT_EQ(scene2Entity2.ySpawn, 1000);
    ASSERT_FALSE(scene2Entity2.isBoss);
    compareEntityConfig(scene2Entity2.entityConfig);
    // scene2Entity3
    ASSERT_EQ(scene2Entity3.xSpawn, 2900);
    ASSERT_EQ(scene2Entity3.ySpawn, 100);
    ASSERT_TRUE(scene2Entity3.isBoss);
    compareEntityConfig(scene2Entity3.entityConfig);
}
