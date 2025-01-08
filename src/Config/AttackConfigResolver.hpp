//
#ifndef _SRC_CONFIG_ATTACKCONFIGRESOLVER_HPP_
#define _SRC_CONFIG_ATTACKCONFIGRESOLVER_HPP_

#include "AttackConfig.hpp"

#include <rengine/Rengine.hpp>

namespace RType {
    namespace Config {

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class AttackConfigResolver
        * @brief Create an attack config from a json path and stores it for future use
        */
        class AttackConfigResolver : public Rengine::Resolver::AResolver<std::string, AttackConfig> {
            public:
                AttackConfigResolver(void) = default;
                ~AttackConfigResolver(void) = default;

            private:
                inline AttackConfig createNewElement(const std::string& key) {
                    return AttackConfig(key);
                }
        };  // class AttackConfigResolver

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class AttackConfigResolverSingletone
        * @brief A singletone of the AttackConfigResolver
        */
        class AttackConfigResolverSingletone {
            public:
                AttackConfigResolverSingletone(void) = default;
                ~AttackConfigResolverSingletone(void) = default;

                /**
                * @fn get
                * @return The same instance of AttackConfigResolver.
                * @brief Get a reference to a static AttackConfigResolver
                */
                static AttackConfigResolver& get(void) noexcept {
                    static AttackConfigResolver res;

                    return res;
                }
        };

    }  // namespace Config
}  // namespace RType


#endif  // _SRC_CONFIG_ATTACKCONFIGRESOLVER_HPP_
