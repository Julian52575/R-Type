//
#ifndef SRC_COMPONENTS_STATS_HPP_
#define SRC_COMPONENTS_STATS_HPP_
#include <cstddef>

#include "../Config/EntityConfig.hpp"

namespace RType {
    namespace Components {

        class Stats {
            public:
                explicit Stats(const Config::EntityConfigStats& stats) : _stats(stats) {}
                ~Stats(void) = default;
                const Config::EntityConfigStats& getStats(void)
                {
                    return this->_stats;
                }

            private:
                Config::EntityConfigStats _stats;
        };  // class Stats
    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_STATS_HPP_
