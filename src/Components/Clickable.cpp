//
#include <functional>
#include <optional>
#include <rengine/Rengine.hpp>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Graphics/Vector.hpp>

#include "src/Components/Clickable.hpp"
#include "Position.hpp"
#include "Hitbox.hpp"
#include "src/Config/EntityConfig.hpp"

namespace RType {
    namespace Components {

        Clickable::Clickable(const std::function<void(void)> fun)
            : _fun(fun)
        {
        }

        void Clickable::componentFunction(Rengine::ECS& ecs, Clickable& thiss, Rengine::Entity& entity)
        {
            std::optional<std::reference_wrapper<Position>> posWrapper = entity.getComponentNoExcept<Position>();
            std::optional<std::reference_wrapper<Hitbox>> hitboxWrapper = entity.getComponentNoExcept<Hitbox>();

            // Ignore if no position or hitbox
            if (posWrapper.has_value() == false || hitboxWrapper.has_value() == false) {
                return;
            }
            std::optional<std::reference_wrapper<const Rengine::Graphics::UserInput>> click
                = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager().receivedInput(Rengine::Graphics::UserInputTypeMouseLeftClick);

            // No click : no function
            if (click.has_value() == false) {
                return;
            }
            const Rengine::Graphics::vector2D<float> pos = posWrapper->get().getVector2D();
            const RType::Config::EntityConfigHitbox& hitbox = hitboxWrapper->get().getSpecs();

            // x position check
            if (!(pos.x + hitbox.offsetFromSpriteOrigin.x <= click->get().data.mousePosition.x
            && click->get().data.mousePosition.x <= pos.x + hitbox.offsetFromSpriteOrigin.x + hitbox.size.x)) {
                return;
            }
            // y position check
            if (!(pos.y + hitbox.offsetFromSpriteOrigin.y <= click->get().data.mousePosition.y
                && click->get().data.mousePosition.y <= pos.y + hitbox.offsetFromSpriteOrigin.y + hitbox.size.y)) {
                return;
            }
            thiss._fun();
        }

    }  // namespace Components
}  // namespace RType
