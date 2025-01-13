//

#include <cstdint>

#include "Relationship.hpp"

namespace RType {
    namespace Components {

        Relationship::Relationship(uint64_t group)
            : _group(group)
        {
        }

        void Relationship::addParent(uint64_t parent) noexcept
        {
            this->_parentVector.push_back(parent);
        }
        void Relationship::removeParent(uint64_t parent) noexcept
        {
            auto it = this->_parentVector.begin();

            while (it != this->_parentVector.end()) {
                if (*it == parent) {
                    break;
                }
                it++;
            } // while it
            // No match found: do nothing
            if (it == this->_parentVector.end()) {
                return;
            }
            this->_parentVector.erase(it);
        }
        uint64_t Relationship::getGroup(void) const noexcept
        {
            return this->_group;
        }
        void Relationship::setGroup(uint64_t parent) noexcept
        {
            this->_group = parent;
        }

        bool Relationship::belong(uint64_t group) const noexcept
        {
            // Same value: must be true
            if (this->_group == group) {
                return true;
            }
            // If not, try binary and
            if (this->_group & group) {
                return true;
            }
            return false;
        }

        bool Relationship::isParented(uint64_t parent) const noexcept
        {
            auto it = this->_parentVector.begin();

            while (it != this->_parentVector.end()) {
                if (*it == parent) {
                    break;
                }
                it++;
            } // while it
            // No match found: return false
            if (it == this->_parentVector.end()) {
                return false;
            }
            return true;
        }

    }  // namespace Components
}  // namespace RType
