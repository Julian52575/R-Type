//
#ifndef SRC_COMPONENTS_CONFIGURATION_HPP_
#define SRC_COMPONENTS_CONFIGURATION_HPP_
#include <cstddef>

#include "../Config/EntityConfig.hpp"

namespace RType {
namespace Components {
class Configuration {
    public:
        explicit Configuration(const RType::Config::EntityConfig& config) : _config(config) {}
        ~Configuration(void) = default;
        const RType::Config::EntityConfig& getConfig(void)
        {
            return this->_config;
        }

    private:
        RType::Config::EntityConfig _config;
};  // class Stats
}  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_CONFIGURATION_HPP_
