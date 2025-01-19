//
#pragma once
#include "src/Config/EntityConfig.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Relationship.hpp"
#include <rengine/src/ECS.hpp>
#include <rengine/src/SparseArray.hpp>

namespace RType {
namespace Components {
class Life {
    public:
        explicit Life(uint16_t maxhp);
        ~Life(void) = default;
        uint16_t getHp(void) const noexcept;
        uint16_t getMaxHp(void) const noexcept;
        void takeDamage(uint16_t damage) noexcept;
        static void componentFunction(Rengine::ECS& ecs, RType::Components::Life& , Rengine::Entity& entity);
    private:
        uint16_t _maxhp;
        uint16_t _hp;
};  // class Life
}  // namespace Components
}  // namespace RType
