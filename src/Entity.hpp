//
#ifndef SRC_ENTITY_HPP_
#define SRC_ENTITY_HPP_
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <optional>
#include "ComponentRegistry.hpp"

namespace Rengine {

    class EntityExceptionComponentNotLinked : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::Entity: Component is not linked to this entity."; };
    };
    class EntityExceptionNotActive : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::Entity: Trying to use a previously destroyed entity."; };
    };
    /**
     * @addtogroup Rengine
     * @namespace Rengine
     * @class Entity
     * @brief A abstraction class holding the index of the associated components in the registry.
     * Note: Do not construct it yourself, get it from the ECS.
     * Note: If the instance of the ECS used to create this entity is destroyed, this entity is destroy alongside it.
    */
    class Entity {
        #define _RENGINEENTITYDESTROYNOOP_ std::function<void(Entity&)>([this](Entity&) {return;})

        public:
            using size_type = typename ComponentRegistry :: size_type;
            Entity(ComponentRegistry& reg, size_type id) : _registry(reg), _id(id) {}
            ~Entity(void) = default;
            /**
            * @fn int
            * @return The index of the component linked to the entity in the SparseArrays.
            */
            operator int(void) const noexcept
            {
                return this->_id;
            }
            /**
            * @fn addComponent
            * @template Component The component class to construct and link to this entity.
            * @template Params The parameter to unpack to the constructor of the Component class.
            * @return A reference to the newly created component, stored in the registry.
            * @exception EntityExceptionNotActive This entity has been previously destroyed.
            * @brief Add a component and link it to this entity.
            * Be careful not to use a reference to an destroyed component.
            */
            template <class Component, class ... Params>
            Component& addComponent(Params &&... args)
            {
                // Previously destroyed entity.
                if (this->_active == false) {
                    throw EntityExceptionNotActive();
                }
                // Try to retrive the SparseArray and emplaceAt
                try {
                    SparseArray<Component>& sp = this->_registry.getComponents<Component>();

                    sp.emplaceAt(this->_id, std::forward<Params>(args)...);
                    return sp[this->_id].value();
                }
                // Component not registred ?
                catch (ComponentRegistryExceptionNotRegistred& e) {
                    throw e;
                }
            }
            /**
            * @fn removeComponent
            * @template Component The component class to remove.
            * @exception ComponentRegistryExceptionNotRegistred Exception raised when Component is not already registered.
            * @exception EntityExceptionNotActive This entity has been previously destroyed.
            * @brief Remove the component linked to this entity.
            */
            template <class Component>
            void removeComponent(void)
            {
                // Previously destroyed entity.
                if (this->_active == false) {
                    throw EntityExceptionNotActive();
                }
                try {
                    SparseArray<Component>& sp = this->_registry.getComponents<Component>();

                    sp.erase(this->_id);
                }
                // Component not registred ?
                catch (ComponentRegistryExceptionNotRegistred &e) {
                    throw e;
                }
            }
            /**
            * @fn removeComponentNoExcept
            * @template Component The component class to remove.
            * @brief this->removeComponent without any exception.
            * Does nothing if Component is not linked to this entity or if this entity has already been destroyed.
            */
            template <class Component>
            void removeComponentNoExcept(void) noexcept
            {
                if (this->_active == false) {
                    return;
                }
                try {
                    SparseArray<Component>& sp = this->_registry.getComponents<Component>();

                    sp.erase(this->_id);
                }
                // Component not registred ?
                catch (ComponentRegistryExceptionNotRegistred &e) {
                    return;
                }
            }
            /**
            * @fn getComponent
            * @template Component The component class to get.
            * @exception EntityExceptionComponentNotLinked This entity has not been linked to this Component class.
            * @exception EntityExceptionNotActive This entity has been previously destroyed.
            * @return A reference to the component linked to this entity.
            * @brief Get the component linked to this entity.
            */
            template <class Component>
            Component& getComponent(void)
            {
                // Previously destroyed entity.
                if (this->_active == false) {
                    throw EntityExceptionNotActive();
                }
                try {
                    SparseArray<Component>& sp = this->_registry.getComponents<Component>();
                    // Out of bound index
                    if (sp.size() < this->_id) {
                        throw EntityExceptionComponentNotLinked();
                    }
                    std::optional<Component>& con = sp[this->_id];
                    // No component for this entity
                    if (con.has_value() == false) {
                        throw EntityExceptionComponentNotLinked();
                    }
                    return con.value();
                }
                // Component not registred in the registry
                catch (ComponentRegistryExceptionNotRegistred& e) {
                    throw EntityExceptionComponentNotLinked();
                }
            }
            /**
            * @fn getComponentNoExcept
            * @template Component The component class to get.
            * @return An optional reference to the component linked to this entity.
            * @brief Get the component linked to this entity. No exception are throw
            */
            template <class Component>
            std::optional<std::reference_wrapper<Component>> getComponentNoExcept(void) noexcept
            {
                if (this->_active == false) {
                    return std::nullopt;
                }
                try {
                    SparseArray<Component>& sp = this->_registry.getComponents<Component>();
                    // Out of bound index
                    if (sp.size() < this->_id) {
                        return std::nullopt;
                    }
                    std::optional<Component>& con = sp[this->_id];
                    // No component for this entity
                    if (con.has_value() == false) {
                        return std::nullopt;
                    }
                    return con.value();
                }
                // Component not registred in the registry
                catch (ComponentRegistryExceptionNotRegistred& e) {
                    return std::nullopt;
                }
            }
            /**
            * @fn setFlag
            * @param flag A 64 unsigned integer.
            * @exception EntityExceptionNotActive This entity has been previously destroyed.
            * @brief Set a int flag to the entity for easing recognition of entity types.
            * Tip : Use binary or to add multiple flag.
            */
            void setFlag(uint64_t flag)
            {
                // Previously destroyed entity.
                if (this->_active == false) {
                    throw EntityExceptionNotActive();
                }
                this->_flag = flag;
            }
            /**
            * @fn getFlag
            * @return flag A 64 unsigned integer.
            * @brief Get the int flag to the entity for easing recognition of entity types.
            * Tip : Use binary or to add multiple flag.
            */
            uint64_t getFlag(void) const noexcept
            {
                return this->_flag;
            }
            /**
            * @fn setComponentsDestroyFunction
            * @exception EntityExceptionNotActive This entity has been previously destroyed.
            * @param fun The function returning a void and taking a reference to this entity.
            * @brief Set the function to be called by this->destroy().
            * This function should remove ALL the linked components to avoid glitches.
            */
            void setComponentsDestroyFunction(const std::function<void(Rengine::Entity &)> fun)
            {
                // Previously destroyed entity.
                if (this->_active == false) {
                    throw EntityExceptionNotActive();
                }
                this->_destroyFunction = fun;
            }
            /**
            * @fn destroyComponents
            * @exception EntityExceptionNotActive This entity has been previously destroyed.
            * @brief Call the destroy function set beforehand by this->setDestroyFunction to remove all linked Component.
            * The entity itself is not destroyed but is considered as such, using it will result in a EntityExceptionNotActive.
            * Note: If this->setDestroyFunction has not been called previously,
            * this function will only set the entity as not active.
            */
            void destroyComponents(void)
            {
                // Previously destroyed entity.
                if (this->_active == false) {
                    throw EntityExceptionNotActive();
                }
                this->_destroyFunction(*this);
                this->_active = false;
            }
            /**
            * @fn isActive
            * @brief Return true if the Entity has not been destroyed by this->destroy.
            */
            bool isActive(void) const noexcept
            {
                return this->_active;
            }

        private:
            size_t _id = (size_t) -1;
            ComponentRegistry& _registry;
            uint64_t _flag = 0;
            std::function<void(Rengine::Entity &)> _destroyFunction = _RENGINEENTITYDESTROYNOOP_;
            bool _active = true;

    };  // class Entity


}  // namespace Rengine
#endif  // SRC_ENTITY_HPP_
