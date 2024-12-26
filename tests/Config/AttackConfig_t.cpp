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

static inline const RType::Config::BuffConfig& getConfigFromType(RType::Config::BuffType type, const std::vector<RType::Config::BuffConfig>& vector)
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

    EXPECT_EQ(config.getType(), RType::Config::AttackType::AttackTypeBuffs);
    EXPECT_EQ(config.getCooldown(), 1.5);
    EXPECT_THROW(config.getMissiles(), RType::Config::AttackConfigExceptionWrongTypeAccess);
    const std::vector<RType::Config::BuffConfig>& vector = config.getBuffs();

    auto& attack = getConfigFromType(RType::Config::BuffType::BuffAttack, vector);
    EXPECT_EQ(attack.getValue(), 1.5);
    EXPECT_EQ(attack.getDuration(), 3.0);

    auto& speed = getConfigFromType(RType::Config::BuffType::BuffSpeed, vector);
    EXPECT_EQ(speed.getValue(), 2.0);
    EXPECT_EQ(speed.getDuration(), 1.0);

    auto& hpHeal = getConfigFromType(RType::Config::BuffType::BuffHpHeal, vector);
    EXPECT_EQ(hpHeal.getValue(), 100);

    auto& hpHealTime = getConfigFromType(RType::Config::BuffType::BuffHpHealTime, vector);
    EXPECT_EQ(hpHealTime.getValue(), 5);
    EXPECT_EQ(hpHealTime.getDuration(), 1.5);

    auto& defense = getConfigFromType(RType::Config::BuffType::BuffDefense, vector);
    EXPECT_EQ(defense.getValue(), 1.5);
    EXPECT_EQ(defense.getDuration(), 1.5);
}
/* Missile json be like
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
                }}}}}}

*/
#define _MISSILEJSONPATH_ "Config/missiles.json"
const RType::Config::MissileConfig& getMissilesConfigFromJsonPath(const std::string& jsonPath,
    const std::vector<RType::Config::MissileConfig>& vector)
{
    auto it = vector.begin();

    while (it != vector.end()) {
        if (it->getJsonPath() == jsonPath) {
            return *it;
        }
        it++;
    }
    throw std::runtime_error("Cannot find jsonPath '" + jsonPath + "'.");
}
TEST(AttackConfig, MissilesConfig)
{
    RType::Config::AttackConfig config(_MISSILEJSONPATH_);

    EXPECT_EQ(config.getType(), RType::Config::AttackType::AttackTypeMissiles);
    EXPECT_THROW(config.getBuffs(), RType::Config::AttackConfigExceptionWrongTypeAccess);
    EXPECT_EQ(config.getCooldown(), 3.0);
    const std::vector<RType::Config::MissileConfig>& vector = config.getMissiles();

    auto& missile = getMissilesConfigFromJsonPath("missile.json", vector);
    std::pair<double, double> missileOffset = {1.1, 1.2};
    std::pair<double, double> missileVelocity = {1.3, 1.4};
    EXPECT_EQ(missile.getOffset(), missileOffset);
    EXPECT_EQ(missile.getVelocity(), missileVelocity);

    auto& missileUp = getMissilesConfigFromJsonPath("missileUp.json", vector);
    std::pair<double, double> missileUpOffset = {2.1, 2.2};
    std::pair<double, double> missileUpVelocity = {2.3, 2.4};
    EXPECT_EQ(missileUp.getOffset(), missileUpOffset);
    EXPECT_EQ(missileUp.getVelocity(), missileUpVelocity);

    auto& missileDown = getMissilesConfigFromJsonPath("missileDown.json", vector);
    std::pair<double, double> missileDownOffset = {-3.1, -3.2};
    std::pair<double, double> missileDownVelocity = {-3.3, -3.4};
    EXPECT_EQ(missileDown.getOffset(), missileDownOffset);
    EXPECT_EQ(missileDown.getVelocity(), missileDownVelocity);
}
