#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>
#include "../../src/Config/AttackConfig.hpp"
#define _BUFFJSONPATH_ "Config/buff.json"
/* The buff.json for reference, if different then trouble
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
            }]}}
*/

const RType::Config::BuffConfig& getConfigFromType(RType::Config::BuffType type, const std::vector<RType::Config::BuffConfig>& vector)
{
    for (auto i = 0; i < vector.size(); i++) {
        if (vector[i].getBuffType() == type) {
            return vector[i];
        }
    }
    throw std::runtime_error("Type was not found in vector.");
}

TEST(AttackConfig, BuffConfig)
{
    RType::Config::AttackConfig config(_BUFFJSONPATH_);

    EXPECT_EQ(config.getType(), RType::Config::AttackType::AttackTypeBuff);
    EXPECT_EQ(config.getCooldown(), 1.5);
    EXPECT_THROW(config.getMissiles(), RType::Config::AttackConfigExceptionAccessShootOnBuffType);
    std::vector<RType::Config::BuffConfig> vector = config.getBuffs();

    auto& attack = getConfigFromType(RType::Config::BuffType::BuffAttack, vector);
    EXPECT_EQ(attack.getValue(), 1.5);
#warning On va faire un test et dire que c'est ok j'ai besoinde dormir
}

