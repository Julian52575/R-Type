//
#ifndef SRC_ENTITY_HPP_
#define SRC_ENTITY_HPP_
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include "ComponentRegistry.hpp"

namespace Rengine {

    /**
     * @addtogroup Rengine
     * @namespace Rengine
     * @class Entity
     * @brief A abstraction class holding the index of the associated components in the registry.
     * Do not construct it yourself, get it from the ECS.
    */
    class Entity {
    #define ENTITYDESTROYNOOP std::function<void(Entity&)>([this](Entity&) {return;})
        public:
            using size_type = typename ComponentRegistry :: size_type;
            Entity(ComponentRegistry& reg, size_type id) : _registry(reg), _id(id) {};
            ~Entity(void) = default;
            /**
            * @fn int
            * @return The index of the component linked to the entity in the SparseArrays.
            */
            operator int(void) const
            {
                return this->_id;
            }
            /**
            * @fn addComponent
            * @template Component The component class to construct and link to this entity.
            * @template Params The parameter to unpack to the constructor of the Component class.
            * @brief Add a component and link it to this entity.
            */
            template <class Component, class ... Params>
            void addComponent(Params &&... args)
            {
                // Try to retrive the SparseArray and emplaceAt
                try {
                    SparseArray<Component>& sp = this->_registry.getComponents<Component>();

                    sp.emplaceAt(this->_id, std::forward<Params>(args)...);
                }
                // Component not registred ?
                catch (ComponentRegistryExceptionNotRegistred& e) {
                    throw e;
                }
            }
            /**
            * @fn removeComponent
            * @template Component The component class to remove.
            * @exception std::runtime_error Exception raised when Component is not already registered.
            * @brief Remove the component linked to this entity.
            */
            template <class Component>
            void removeComponent(void)
            {
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
            * @fn setFlag
            * @param flag A 64 unsigned integer.
            * @brief Set a int flag to the entity for easing recognition of entity types.
            * Tip : Use binary or to add multiple flag.
            */
            void setFlag(uint64_t flag)
            {
                this->_flag = flag;
            }
            /**
            * @fn getFlag
            * @return flag A 64 unsigned integer.
            * @brief Get the int flag to the entity for easing recognition of entity types.
            * Tip : Use binary or to add multiple flag.
            */
            uint64_t getFlag(void) const
            {
                return this->_flag;
            }
            /**
            * @fn setDestroyFunction
            * @param fun The function returning a void and taking a reference to this entity.
            * @brief Set the function to be called when this->destroy() is called.
            */
            void setDestroyFunction(const std::function<void(Entity &)> fun)
            {
                this->_destroyFunction = fun;
            }
            /**
            * @fn destroyAllComponents
            * @brief Call the destroy function set beforehand by this->setDestroyFunction.
            * Calling this function results in a No-Op starting from the 2nd call to avoid invalid erasure.
            * Note: Does not call the destructor of this class.
            * Note: If no destroy function has been set, this does nothing.
            */
            void destroyAllComponents(void)
            {
                this->_destroyFunction(*this);
            }

        private:
            size_t _id = (size_t) -1;
            ComponentRegistry& _registry;
            uint64_t _flag = 0;
            std::function<void(Entity &)> _destroyFunction = ENTITYDESTROYNOOP;

    };  // class Entity


}  // namespace Rengine
#endif  // SRC_ENTITY_HPP_
