/*
*/

#ifndef SRC_ECS_HPP_
#define SRC_ECS_HPP_
#include "ComponentRegistry.hpp"
#include "Entity.hpp"
#include "SparseArray.hpp"
#include <cstddef>
#include <memory>

namespace Rengine {

    class ECSExceptionEntityNotFound : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::ECS: The provided entity is not registered in the ECS."; };
    };
    class ECSExceptionCannotAddEntityLimit : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::ECS: Cannot add entity, the limit has been reached."; };
    };
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
            ECS(void)
            {
                this->_currentEntities.addSize(this->_sparseArrayDefaultSize);
            };
            /**
            * @fn ECS
            * @param entityLimit The default amount of entity usable.
            * @brief Create a new instance of ECS.
            */
            ECS(size_type entityLimit) : _sparseArrayDefaultSize(entityLimit)
            {
                this->_currentEntities.addSize(this->_sparseArrayDefaultSize);
            };
            /**
            * @fn ~ECS
            * @brief Destroy an instance of ECS.
            */
            ~ECS(void) = default;
            /**
            * @fn addEntity
            * @exception ECSExceptionCannotAddEntityLimit The entity limit has been reached and no place is available.
            * @exception ECSE
            * @brief Add a blank entity to the ECS.
            */
            Entity& addEntity(void)
            {
                for (size_type i = 0; i < this->_currentEntities.size(); i++) {
                    // If Entity i exists, continue
                    if (this->_currentEntities[i].has_value() == true) {
                        continue;
                    }
                    this->_currentEntities[i].emplace(this->_registry, i);
                    return this->_currentEntities[i].value();
                }
                // Every entity is already used
                throw ECSExceptionCannotAddEntityLimit();
            }
            /**
            * @fn removeEntity
            * @param Entity The entity to remove. Note that using that entity again WILL result in a glitch. :(
            * @exception ECSExceptionEntityNotFound The provided entity is not registred in the ECS.
            * @brief Remove an entity from the ECS.
            */
            void removeEntity(const Entity& en)
            {
                size_type idx = this->_currentEntities.getIndex(en);

                if (idx == static_cast<size_type>(-1)) {
                    throw ECSExceptionEntityNotFound();
                }
                if (this->_currentEntities[idx].has_value() == false) {
                    throw ECSExceptionEntityNotFound();
                }
                this->_currentEntities[idx].value().destroyAllComponents();
                this->_currentEntities[idx].reset();
            }
            /**
            * @fn getEntity
            * @param idx The index of the entity to get.
            * @return A reference to an entity.
            * @exception ECSExceptionEntityNotFound The asked entity is not registred in the ECS.
            * @brief Get the reference of the entity at index idx.
            */
            Entity& getEntity(size_type idx)
            {
                if (idx > this->_currentEntities.size()) {
                    throw ECSExceptionEntityNotFound();
                }
                if (this->_currentEntities[idx].has_value() == false) {
                    throw ECSExceptionEntityNotFound();
                }
                return this->_currentEntities[idx].value();
            }
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
            SparseArray<Entity> _currentEntities;

    };  // class ECS
}  // namespace Rengine
#endif  // SRC_ECS_HPP_
