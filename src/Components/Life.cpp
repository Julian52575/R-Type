#include "Life.hpp"

namespace RType {
    namespace Components {

        Life::Life(uint16_t maxhp) : _maxhp(maxhp), _hp(maxhp)
        {
        }

        uint16_t Life::getHp(void) const noexcept
        {
            return this->_hp;
        }

        uint16_t Life::getMaxHp(void) const noexcept
        {
            return this->_maxhp;
        }

        void Life::setHp(uint16_t hp) noexcept
        {
            if (hp > this->_maxhp) {
                this->_hp = this->_maxhp;
            } else {
                this->_hp = hp;
            }
        }

        void Life::setMaxHp(uint16_t maxhp) noexcept
        {
            this->_maxhp = maxhp;
        }

        void Life::takeDamage(uint16_t damage) noexcept
        {
            if (damage > this->_hp) {
                this->_hp = 0;
            } else {
                this->_hp -= damage;
            }
        }

        void Life::componentFunction(Rengine::ECS& ecs, RType::Components::Life& , Rengine::Entity& entity) {
            std::optional<std::reference_wrapper<Life>> life = entity.getComponentNoExcept<Life>();

            if (!life) {
                return;
            }
            if (life->get()._hp <= 0) {
                ecs.removeEntity(entity);
            }
        }
    }  // namespace Components
}  // namespace RType