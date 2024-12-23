/*
*/

#ifndef SRC_ECS_HPP_
#define SRC_ECS_HPP_
#include "ComponentRegistry.hpp"
#include "Entity.hpp"
#include "SparseArray.hpp"

namespace Rengine {
    /**
     * @addtogroup Rengine
     * @namespace Rengine
     * @class ECS
     * @brief The main class of the game engine. Everything ECS related passes by this class.
    */
    class ECS {
        public:
            using size_type = typename ComponentRegistry :: size_type;
            /**
            * @fn ECS
            * @brief Create a new instance of ECS.
            */
            ECS(void) = default;
            /**
            * @fn ECS
            * @param size The default size of every underlying SparseArray.
            * @brief Create a new instance of ECS.
            */
            ECS(size_type size) : _sparseArrayDefaultSize(size) {};
            /**
            * @fn ~ECS
            * @brief Destroy an instance of ECS.
            */
            ~ECS(void) = default;
            /**
            * @fn addEntity
            * @brief Add a blank entity to the ECS.
            */
            Entity& addEntity(void);
            /**
            * @fn removeEntity
            * @param Entity The entity to remove. Note that using that entity again WILL result in a glitch. :(
            * @brief Remove an entity from the ECS.
            */
            void removeEntity(Entity&);
            /**
            * @fn registerComponent
            * @template Component The type to be registered in the registry.
            * @return SparseArray<Component>& A Rengine::SparseArray of your templated class.
            * @exception std::runtime_error Exception raised when Component is already registered.
            * @brief Create a new SparseArray of the templated type and stores it.
            */
            template <class Component>
            SparseArray<Component>& registerComponent(void)
            {
                try {
                    return this->_registry.registerComponent<Component>(this->_sparseArrayDefaultSize);
                } catch (ComponentRegistryExceptionAlreadyRegistred& e) {
                    return this->_registry.getComponents<Component>();
                }
            }
            /**
            * @fn getComponents
            * @template Component The type you want to retrive from the registry.
            * @return SparseArray<Component>& A Rengine::SparseArray of your templated class.
            * @exception std::runtime_error Exception raised when Component is not registered.
            * @brief Retrive the Rengine::SparseArray associated with the templated type.
            */
            template <class Component>
            SparseArray<Component>& getComponents(void)
            {
                try {
                    return this->_registry.registerComponent<Component>();
                } catch (ComponentRegistryExceptionAlreadyRegistred&) {
                    return this->_registry.getComponents<Component>();
                }
            }
            /**
            * @fn getComponents
            * @template Component The type you want to retrive from the registry.
            * @return SparseArray<Component>& A Rengine::SparseArray of your templated class.
            * @exception std::runtime_error Exception raised when Component is not registered.
            * @brief Retrive the Rengine::SparseArray associated with the templated type.
            */
            template <class Component>
            SparseArray<Component>& getComponents(void) const
            {
                try {
                    return this->_registry.registerComponent<Component>();
                } catch (ComponentRegistryExceptionAlreadyRegistred&) {
                    return this->_registry.getComponents<Component>();
                }
            }

        private:
            ComponentRegistry _registry;
            #define DEFAULTSPARSEARRAYSIZE 100
            size_type _sparseArrayDefaultSize = DEFAULTSPARSEARRAYSIZE;
    };  // class ECS
}  // namespace Rengine
#endif  // SRC_ECS_HPP_
