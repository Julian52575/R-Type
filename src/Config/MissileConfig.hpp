//
#ifndef SRC_CONFIG_MISSILECONFIG_HPP_
#define SRC_CONFIG_MISSILECONFIG_HPP_
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

#include "./AttackBuffTypeEnum.hpp"

namespace RType {

    namespace Config {

        enum MissileControlType {
            MissileControlTypeNA,
            MissileControlTypeVelocity,
            MissileControlTypeScript,
            MissileControlTypeUserInput
        };

        class MissileConfigException : public std::exception {
            public:
                explicit MissileConfigException(const std::string& msg)
                {
                    this->_msg = "RType::Config::MissileConfig: " + msg;
                }
                ~MissileConfigException(void) = default;
                const char *what() const noexcept {
                    return this->_msg.c_str();
                }

            private:
                std::string _msg;
        };

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
                    if (missiles.contains("controlType") == false) {
                        throw MissileConfigException("No 'controlType' field on missile.");
                    }
                    // Parse controlType
                    std::string controlType = missiles["controlType"];

                    if (controlType == "script") {
                        // luaScript
                        this->_controlType = MissileControlType::MissileControlTypeScript;
                        this->_scriptPath = missiles["scriptPath"];
                    } else if (controlType == "velocity") {
                        // velocity
                        this->_controlType = MissileControlType::MissileControlTypeVelocity;
                        this->_velocity = {missiles["velocity"]["x"], missiles["velocity"]["y"]};
                    } else if (controlType == "userInput") {
                        // userInput
                        this->_controlType = MissileControlType::MissileControlTypeUserInput;
                    } else {
                        throw MissileConfigException("Unknow control :" + controlType);
                    }
                    // Parse json
                    this->_jsonPath = missiles["json"];
                    // Parse offset
                    this->_offset = {missiles["offset"]["x"], missiles["offset"]["y"]};
                    if (missiles.contains("timeLimit") == true) {
                        this->_timeLimit = missiles["timeLimit"];
                    }
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
                MissileControlType getControlType(void) const noexcept
                {
                    return this->_controlType;
                }
                const std::string& getScriptPath(void) const noexcept
                {
                    return this->_scriptPath;
                }
                float getTimeLimitSeconds(void) const noexcept
                {
                    return this->_timeLimit;
                }

            private:
                MissileControlType _controlType;
                std::string _jsonPath;
                std::string _scriptPath;
                std::pair<double, double> _offset = {0, 0};
                std::pair<double, double> _velocity = {1, 1};
                float _timeLimit = 0.0f;
        };

    }  // namespace Config
}  // namespace RType
#endif  // SRC_CONFIG_MISSILECONFIG_HPP_
