//
#ifndef _SRC_GRAPHICS_VECTOR2_HPP_
#define _SRC_GRAPHICS_VECTOR2_HPP_
#include <cstdint>
#include <type_traits>
#include <iostream>

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
        template <class I>
        std::ostream& operator<<(std::ostream& os, const vector2D<I>& vec)
        {
            os << "{" << vec.x << "x," << vec.y << "y}";
            return os;
        }
        template <class I>
        bool operator==(const vector2D<I>& a, const vector2D<I>& b)
        {
            if (a.x != b.x) {
                return false;
            }
            if (a.y != b.y) {
                return false;
            }
            return true;
        }
        template <class I>
        bool operator!=(const vector2D<I>& a, const vector2D<I>& b)
        {
            return !(a == b);
        }

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
            static_assert(std::is_integral<providedInteger>::value || std::is_floating_point_v<providedInteger>);
            providedInteger x;
            providedInteger y;
            providedInteger z;
        };
        template <class I>
        bool operator==(const vector3D<I>& a, const vector3D<I>& b)
        {
            if (a.x != b.x) {
                return false;
            }
            if (a.y != b.y) {
                return false;
            }
            if (a.z != b.z) {
                return false;
            }
            return true;
        }
        template <class I>
        bool operator!=(const vector3D<I>& a, const vector3D<I>& b)
        {
            return !(a == b);
        }
        template <class I>
        bool operator==(const vector3D<I>& a, const vector2D<I>& b)
        {
            if (a.x != b.x) {
                return false;
            }
            if (a.y != b.y) {
                return false;
            }
            return true;
        }
        template <class I>
        bool operator!=(const vector3D<I>& a, const vector2D<I>& b)
        {
            return !(a == b);
        }

        template <class I>
        std::ostream& operator<<(std::ostream& os, const vector3D<I>& vec)
        {
            os << "{" << vec.x << "x," << vec.y << "y," << vec.z << "z}";
            return os;
        }

    };
};
#endif
