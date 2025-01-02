//
#ifndef _SRC_CONFIG_BUFFCONFIG_HPP_
#define _SRC_CONFIG_BUFFCONFIG_HPP_
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "./Enum.hpp"

namespace RType {

    namespace Config {

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class BuffConfig
        * @brief Convert the "buffs" field into a c++ class.
        */
        class BuffConfig {
            public:
                BuffConfig(void) = default;
                BuffConfig(nlohmann::json& buff) {
                    // Parse buff type
                    std::vector<std::pair<std::string, RType::Config::BuffType>> bind = {
                        {"attack", RType::Config::BuffType::BuffAttack},
                        {"speed", RType::Config::BuffType::BuffSpeed},
                        {"hpHeal", RType::Config::BuffType::BuffHpHeal},
                        {"hpHealTime", RType::Config::BuffType::BuffHpHealTime},
                        {"defense", RType::Config::BuffType::BuffDefense}
                    };
                    std::string type = buff["type"];
                    auto it = bind.begin();
                    // loop over the bind vector
                    while (it != bind.end()) {
                        // Type found ? Break loop
                        if (it->first == type) {
                            break;
                        }
                        it++;
                    }
                    // If it reach the end, exception
                    if (it == bind.end()) {
                        throw std::runtime_error("Unknow type.");
                    }
                    this->_buffType = it->second;
                    // Parse value
                    this->_value = buff["value"];
                    // Parse duration
                    if (this->_buffType != RType::Config::BuffType::BuffHpHeal) {
                        this->_duration = buff["duration"];
                    }
                }
                ~BuffConfig(void) = default;
                RType::Config::BuffType getBuffType(void) const noexcept
                {
                    return this->_buffType;
                }
                double getDuration(void) const noexcept
                {
                    return this->_duration;
                }
                double getValue(void) const noexcept
                {
                    return this->_value;
                }

            private:
                RType::Config::BuffType _buffType = RType::Config::BuffType::BuffNA;
                double _duration = 0.0;
                double _value = 0.0;
        };

    }  // namespace Config

}  // namespace RType
#endif
