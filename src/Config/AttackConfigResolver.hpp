//
#ifndef SRC_CONFIG_ATTACKCONFIGRESOLVER_HPP_
#define SRC_CONFIG_ATTACKCONFIGRESOLVER_HPP_

#include <rengine/Rengine.hpp>
#include <string>

#include "AttackConfig.hpp"

namespace RType {
    namespace Config {

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class AttackConfigResolver
        * @brief Create and store AttackConfig object for faster load.
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
        * @brief A class holding a singletone instance of AttackConfigResolver
        */
        class AttackConfigResolverSingletone {
            public:
                AttackConfigResolverSingletone(void) = default;
                ~AttackConfigResolverSingletone(void) = default;

                /**
                * @fn get
                * @return The singletone instance of AttackConfigResolver.
                * @brief Get a reference to a static AttackConfigResolver
                */
                static AttackConfigResolver& get(void) noexcept {
                    static AttackConfigResolver res;

                    return res;
                }
        };

    }  // namespace Config
}  // namespace RType
#endif  // SRC_CONFIG_ATTACKCONFIGRESOLVER_HPP_
