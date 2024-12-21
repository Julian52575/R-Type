#ifndef SRC_COMPONENTDATA_HPP_
#define SRC_COMPONENTDATA_HPP_
#include <any>
#include <cstdint>
#include <functional>
#include "./ComponentRegistry.hpp"

namespace Rengine {
    class ComponentRegistry;

    struct ComponentData {
        public:
            using function_type = std::function<void(Rengine::ComponentRegistry&)>;
            // sparse array Type
            std::any _componentSparseArray;
            std::optional<function_type> _componentFunction;
    };  // struct componentData

};  // namespace Rengine

#endif