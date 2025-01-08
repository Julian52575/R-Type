//
#ifndef SRC_CONFIG_MISSILECONFIG_HPP_
#define SRC_CONFIG_MISSILECONFIG_HPP_
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

#include "./AttackBuffTypeEnum.hpp"

namespace RType {

    namespace Config {

    /**
    * @addtogroup RType::Config
    * @namespace Config
    * @class MissileConfig
    * @brief Convert the "missiles" field into a c++ class.
    */
    class MissileConfig {
        public:
            MissileConfig(void) = default;
            explicit MissileConfig(nlohmann::json& missiles) {
                // Parse json
                this->_jsonPath = missiles["json"];
                // Parse offset
                this->_offset = {missiles["offset"]["x"], missiles["offset"]["y"]};
                // Parse velocity
                this->_velocity = {missiles["velocity"]["x"], missiles["velocity"]["y"]};
            }
            ~MissileConfig(void) = default;
            const std::string& getJsonPath(void) const noexcept
            {
                return this->_jsonPath;
            }
            const std::pair<double, double>& getOffset(void) const noexcept
            {
                return this->_offset;
            }
            const std::pair<double, double>& getVelocity(void) const noexcept
            {
                return this->_velocity;
            }

        private:
            std::string _jsonPath;
            std::pair<double, double> _offset = {0, 0};
            std::pair<double, double> _velocity = {1, 1};
    };

    }  // namespace Config
}  // namespace RType
#endif  // SRC_CONFIG_MISSILECONFIG_HPP_
