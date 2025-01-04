//
#ifndef _SRC_GRAPHICS_VECTOR2_HPP_
#define _SRC_GRAPHICS_VECTOR2_HPP_
#include <cstdint>
#include <type_traits>

namespace Rengine {
    namespace Graphics {
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class vector2D
        * @template providedInteger A integer class derived from int.
        * @brief A struct of 2 integer.
        */
        template <class providedInteger>
        struct vector2D {
            // Ensure the templated class is either a integer or a floating number
            static_assert(std::is_integral<providedInteger>::value || std::is_floating_point_v<providedInteger>);
            providedInteger x;
            providedInteger y;
        };

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class vector3D
        * @template providedInteger A integer class derived from int.
        * @brief A struct of 3 integer.
        */
        template <class providedInteger>
        struct vector3D {
            // Ensure the templated class is derived from int
            static_assert(std::is_base_of<int, providedInteger>::value);
            providedInteger x;
            providedInteger y;
            providedInteger z;
        };
    };
};
#endif
