//
#include "IGraphics.hpp"

namespace Rengine {

    namespace Graphics {
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class ISprite
        * @brief The interface to follow when implementing sprites.
        */
        class ISprite {
            public:
                virtual ~ISprite(void) = default;
                /**
                * @fn draw
                * @param Entity The entity to remove. Note that using that entity again WILL result in a glitch. :(
                * @exception ECSExceptionEntityNotFound The provided entity is not registred in the ECS.
                * @brief Remove an entity from the ECS.
                */
                virtual void draw(void) = 0;


        };

    }  // namespace Graphics
}  // namespace Rengine
