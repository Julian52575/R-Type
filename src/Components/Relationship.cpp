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
        }  // while it
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
    bool Relationship::isParent(uint64_t id) const noexcept
    {
        if (this->_parentVector.size() == 0) {
            return false;
        }
        // Check parents
        auto pIt = this->_parentVector.begin();
        while (pIt != this->_parentVector.end()) {
            if (*pIt == id) {
                break;
            }
            pIt++;
        }  // while it
        //  Match found: return true
        if (pIt != this->_parentVector.end()) {
            return true;
        }
        return false;
    }
    bool Relationship::isChild(uint64_t id) const noexcept
    {
        if (this->_childVector.size() == 0) {
            return false;
        }
        // Check children
        auto cIt = this->_childVector.begin();
        while (cIt != this->_childVector.end()) {
            if (*cIt == id) {
                break;
            }
            cIt++;
        }
        if (cIt != this->_childVector.end()) {
            return true;
        }
        return false;
    }
    bool Relationship::isRelated(uint64_t familyMember) const noexcept
    {
        if (this->isChild(familyMember) == true) {
            return true;
        }
        if (this->isParent(familyMember) == true) {
            return true;
        }
        return false;
    }
    bool Relationship::isKinless(void) const noexcept
    {
        return this->_parentVector.size() == 0 && this->_childVector.size() == 0;
    }
    const std::vector<uint64_t>& Relationship::getParents(void) const noexcept
    {
        return this->_parentVector;
    }
    void Relationship::addChild(uint64_t child) noexcept
    {
        this->_childVector.push_back(child);
    }
    void Relationship::removeChild(uint64_t child) noexcept
    {
        auto it = this->_childVector.begin();
        while (it != this->_childVector.end()) {
            if (*it == child) {
                break;
            }
            it++;
        }  // while it
        // No match found: do nothing
        if (it == this->_childVector.end()) {
            return;
        }
        this->_childVector.erase(it);
    }
}  // namespace Components
}  // namespace RType
