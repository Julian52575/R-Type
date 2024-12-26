//
#ifndef _SRC_CONFIG_ENUM_HPP_
#define _SRC_CONFIG_ENUM_HPP_

namespace RType {

    namespace Config {
        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @enum AttackType
        * @brief An enum of the different type of attack.
        */
        enum AttackType {
            AttackTypeNA = 0x00,
            AttackTypeShoot = 0x01,
            AttackTypeBuff = 0x02
        };
        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @enum Buff
        * @brief An enum of the different type of buff.
        */
        enum BuffType {
            BuffNA = 0x00,
            BuffAttack = 0x01,
            BuffDefense = 0x02,
            BuffHpHeal = 0x03,
            BuffSpeed = 0x04,
            BuffHpHealTime = 0x05
        };

    }  // namespace Config
}  // namespace RType
#endif  //_SRC_CONFIG_ENUM_HPP_
