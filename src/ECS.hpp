/*
*/

#ifndef SRC_ECS_HPP_
#define SRC_ECS_HPP_

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "ComponentRegistry.hpp"
#include "Entity.hpp"
#include "SparseArray.hpp"

namespace Rengine {

    class ECSExceptionComponentNotRegistred : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::ECS: Trying to access an unregistered component."; };
    };
    class ECSExceptionEntityNotFound : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::ECS: The asked entity is not registered in the ECS."; };
    };
    class ECSExceptionCannotAddEntityLimit : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::ECS: Cannot add entity, the limit has been reached."; };
    };
    class ECSExceptionNoComponentFunction : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::ECS: No component function has been set."; };
    };
    class ECSExceptionBadComponentFunctionType : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::ECS: The component function and the provided template have different type definition."; };
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
            explicit ECS(size_type entityLimit) : _sparseArrayDefaultSize(entityLimit)
            {
                this->_currentEntities.addSize(this->_sparseArrayDefaultSize);
            };
            /**
            * @fn ~ECS
            * @brief Destroy an instance of ECS.
            */
            ~ECS(void)
            {
                for (auto eit : this->_currentEntities) {
                    if (eit.has_value() == false) {
                        continue;
                    }
                    try {
                        this->removeEntity(*eit);
                    }
                    // Ignore previously destroyed entities
                    catch (EntityExceptionNotActive &e) {
                        continue;
                    }
                }
            }
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
            * @param Entity The entity to remove. Note that using a reference entity again WILL result in a glitch. :(
            * @exception ECSExceptionEntityNotFound The provided entity is not registred in the ECS.
            * @brief Remove an entity and its components from the ECS.
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
                this->_currentEntities[idx].value().destroyComponents();
                this->_currentEntities[idx].reset();
            }
            /**
            * @fn removeEntity
            * @param idx The index of the entity to remove
            * @exception ECSExceptionEntityNotFound The asked entity is not registred in the ECS.
            * @brief Remove an entity and its components from the ECS.
            */
            void removeEntity(Rengine::ECS::size_type idx)
            {
                if (idx == static_cast<size_type>(-1)) {
                    throw ECSExceptionEntityNotFound();
                }
                if (this->_currentEntities[idx].has_value() == false) {
                    throw ECSExceptionEntityNotFound();
                }
                this->_currentEntities[idx].value().destroyComponents();
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
            * @brief Create a new SparseArray of the templated type and stores it.
            * If the component is already registered, this function returns a reference to it.
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
            SparseArray<Component>& getComponents(void) const
            {
                try {
                    return this->_registry.registerComponent<Component>();
                } catch (ComponentRegistryExceptionNotRegistred& e) {
                    return this->_registry.getComponents<Component>();
                }
            }
            /**
            * @fn setComponentFunction
            * @template Component The type you want to bind to the function.
            * @brief Bind a function to a type for future use.
            * This function takes a reference to the ECS and a reference to an entity and its Component.
            * The ECS will call this function for each Component stored.
            */
            template<class Component>
            void setComponentFunction(const std::function<void(ECS&, Component&, Entity&)> fun)
            {
                auto i = std::type_index(typeid(Component));

                this->_functionArray[i] = fun;
            }
            /**
            * @fn setComponentFunction
            * @template Component The type you want to bind to the function.
            * @template Parameters The additional parameters to the component function
            * @brief Bind a function to a type for future use.
            * This function takes a reference to the ECS and a reference to an entity and its Component.
            * The ECS will call this function for each Component stored.
            */
            template<class Component, class ... Parameters>
            void setComponentFunction(const std::function<void(ECS&, Component&, Entity&, Parameters&&...)>& fun)
            {
                auto i = std::type_index(typeid(Component));

                this->_functionArray[i] = fun;
            }
            /**
            * @fn runComponentFunction
            * @template Component The type you want to run.
            * @brief Run the function, previously set with this->setComponentFunction, on each entity and its component of the templated type.
            */
            template<class Component>
            void runComponentFunction(void)
            {
                auto i = std::type_index(typeid(Component));
                auto it = this->_functionArray.find(i);

                // No function ?
                if (it == this->_functionArray.end()) {
                    throw ECSExceptionNoComponentFunction();
                }
                // Try to:
                // Retrive function
                // Parse entity list
                // Call function with each component / entity
                try {
                    std::function<void(ECS&, Component&, Entity&)> fun = std::any_cast<std::function<void(ECS&, Component&, Entity&)>>(it->second);

                    for (auto eit : this->_currentEntities) {
                        // Ignore uninitialised entities
                        if (eit.has_value() == false) {
                            continue;
                        }
                        try {
                            Component& con = eit->getComponent<Component>();

                            fun(*this, con, *eit);
                        }
                        // Ignore entities that do not have this Component
                        catch (EntityExceptionComponentNotLinked& e) {
                            continue;
                        }
                    }
                // Any cast fail.
                } catch (std::bad_any_cast &e) {
                    throw ECSExceptionBadComponentFunctionType();
                }
            }
            /**
            * @fn runComponentFunction
            * @template Component The type you want to run.
            * @template Parameters The additional parameters for the component function
            * @brief Run the function, previously set with this->setComponentFunction, on each entity and its component of the templated type.
            */
            template<class Component, class ... Parameters>
            void runComponentFunction(Parameters&& ... params)
            {
                auto i = std::type_index(typeid(Component));
                auto it = this->_functionArray.find(i);

                // No function ?
                if (it == this->_functionArray.end()) {
                    throw ECSExceptionNoComponentFunction();
                }
                // Try to:
                // Retrive function
                // Parse entity list
                // Call function with each component / entity
                try {
                    std::function<void(ECS&, Component&, Entity&, Parameters&&...)> fun =
                        std::any_cast<std::function<void(ECS&, Component&, Entity&, Parameters&&...)>>(it->second);

                    for (auto eit : this->_currentEntities) {
                        // Ignore uninitialised entities
                        if (eit.has_value() == false) {
                            continue;
                        }
                        try {
                            Component& con = eit->getComponent<Component>();

                            fun(*this, con, eit.value(), std::forward<Parameters>(params)...);
                        }
                        // Ignore entities that do not have this Component
                        catch (EntityExceptionComponentNotLinked& e) {
                            continue;
                        }
                    }
                // Any cast fail.
                } catch (std::bad_any_cast &e) {
                    throw ECSExceptionBadComponentFunctionType();
                }
            }

        private:
            ComponentRegistry _registry;
            #define DEFAULTSPARSEARRAYSIZE 100
            size_type _sparseArrayDefaultSize = DEFAULTSPARSEARRAYSIZE;
            SparseArray<Entity> _currentEntities;
            //                      Type    - std::function<void(ECS&, Component&, Entity&)>
            std::unordered_map<std::type_index, std::any> _functionArray;
    };  // class ECS
}  // namespace Rengine
#endif  // SRC_ECS_HPP_
