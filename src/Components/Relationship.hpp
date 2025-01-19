//
#ifndef SRC_COMPONENTS_RELATIONSHIP_HPP_
#define SRC_COMPONENTS_RELATIONSHIP_HPP_

#include <cstdint>
#include <vector>

namespace RType {
namespace Components {
/**
* @addtogroup RType::Components
* @namespace Components
* @class Relationship
* @brief This component manage the relationship between entites.
*/
class Relationship {
    public:
        /**
        * @fn Relationship
        * @param group The group this entity belong to. Binary and allows you to add multiple group in a single digit.
        * @brief Create the relationship component.
        */
        explicit Relationship(uint64_t group = 0);
        ~Relationship(void) = default;
        /**
        * @fn addParent
        * @param parent The parent entity to add.
        * @brief Add a parent to the list.
        * Note: The parent is added even if it's alreay present, it's more optimized than iterating over the parent list.
        */
        void addParent(uint64_t parent) noexcept;
        /**
        * @fn addChild
        * @param parent The child entity to add.
        * @brief Add a child to the list.
        * Note: The child is added even if it's alreay present, it's more optimized than iterating over the children list.
        */
        void addChild(uint64_t child) noexcept;
        /**
        * @fn removeParent
        * @param parent The parent entity to remove.
        * @brief Remove the parent from the list.
        * Note: Does nothing if the parent isn't in the list.
        */
        void removeParent(uint64_t parent) noexcept;
        /**
        * @fn removeChild
        * @param child The child entity to remove.
        * @brief Remove the child from the list.
        * Note: Does nothing if the child isn't in the list.
        */
        void removeChild(uint64_t child) noexcept;

    public:
        /**
        * @fn getGroup
        * @return uint64_t The entity's group.
        * @brief Return the entity's group.
        */
        uint64_t getGroup(void) const noexcept;
        /**
        * @fn setGroup
        * @param group The new entity group, represented by a number.
        * @brief Set the entity group.
        */
        void setGroup(uint64_t group) noexcept;
        /**
        * @fn belong
        * @param group The group to Check.
        * @return true if the entity belong, false otherwise.
        * @brief Check if the entity belong to the group.
        */
        bool belong(uint64_t group) const noexcept;
        /**
        * @fn isRelated
        * @param id The id to check.
        * @return true if the id is a child or parent, false otherwise.
        * @brief Check if the id is related.
        */
        bool isRelated(uint64_t id) const noexcept;
        /**
        * @fn isParent
        * @param id The id to check.
        * @return true if is a parent, false otherwise.
        * @brief Check if a id is a parent..
        */
        bool isParent(uint64_t id) const noexcept;
        /**
        * @fn isChild
        * @param id The id to check.
        * @return true if is a child, false otherwise.
        * @brief Check if a id is a child.
        */
        bool isChild(uint64_t id) const noexcept;
        /**
        * @fn isKinless
        * @return true if the entity has no parent or child, false otherwise
        * @brief Check if the entity has no parent and no children.
        * https://en.wikipedia.org/wiki/Kinlessness
        */
        bool isKinless(void) const noexcept;
        /**
        * @fn getParents
        * @return A vector of the entity's parent(s)
        * @brief Get the entity's parent(s)
        */
        const std::vector<uint64_t>& getParents(void) const noexcept;

    private:
        uint64_t _group;
        std::vector<uint64_t> _parentVector;
        std::vector<uint64_t> _childVector;
};
}  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_RELATIONSHIP_HPP_
