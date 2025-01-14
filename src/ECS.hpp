/*
*/

#ifndef SRC_ECS_HPP_
#define SRC_ECS_HPP_

#include <cstddef>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <typeindex>

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
            ECSExceptionNoComponentFunction(const char *componentName)
            {
                std::string name = componentName;
                std::string foot = "'.";

                this->_concat = this->_concat + name + foot;
            }
            const char *what() const noexcept
            {
                return this->_concat.c_str();
            };

        private:
            std::string _concat = "Rengine::ECS: No component function has been set for type '";
    };
    class ECSExceptionBadComponentFunctionType : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::ECS: The component function and the provided template have different type definition."; };
    };
    class ECSException : public std::exception {
        public:
            ECSException(const std::string& msg) : _msg("Rengine::ECS: " + msg) {};
            const char *what() const noexcept
            {
                return this->_msg.c_str();
            };

        private:
            std::string _msg;
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
                    this->_currentEntitiesCount += 1;
                    if (i > this->_maxEntityId) {
                        this->_maxEntityId = i;
                    }
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
                this->_currentEntitiesCount -= 1;
                this->updateMaxEntityId();
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
                this->_currentEntitiesCount -= 1;
                this->updateMaxEntityId();
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
            * @exception ECSExceptionComponentNotRegistred The component was not previously registred.
            * @brief Retrive the Rengine::SparseArray associated with the templated type.
            */
            template <class Component>
            SparseArray<Component>& getComponents(void)
            {
                try {
                    return this->_registry.getComponents<Component>();
                } catch (ComponentRegistryExceptionNotRegistred) {
                    throw ECSExceptionComponentNotRegistred();
                }
            }
            /**
            * @fn getComponents
            * @template Component The type you want to retrive from the registry.
            * @exception ECSExceptionComponentNotRegistred The component was not previously registred.
            * @return SparseArray<Component>& A Rengine::SparseArray of your templated class.
            * @brief Retrive the Rengine::SparseArray associated with the templated type.
            */
            template <class Component>
            SparseArray<Component>& getComponents(void) const
            {
                try {
                    return this->_registry.getComponents<Component>();
                } catch (ComponentRegistryExceptionNotRegistred) {
                    throw ECSExceptionComponentNotRegistred();
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
                    throw ECSExceptionNoComponentFunction(i.name());
                }
                // Try to:
                // Retrive function
                // Parse entity list
                // Call function with each component / entity
                try {
                    std::function<void(ECS&, Component&, Entity&)> fun = std::any_cast<std::function<void(ECS&, Component&, Entity&)>>(it->second);

                    for (size_type i = 0; i <= this->_maxEntityId; i++) {
                        // Ignore uninitialised entities
                        if (this->_currentEntities[i].has_value() == false) {
                            continue;
                        }
                        try {
                            Component& con = this->_currentEntities[i]->getComponent<Component>();

                            fun(*this, con, this->_currentEntities[i].value());
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
                    throw ECSExceptionNoComponentFunction(i.name());
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
            /**
            * @fn getEntityLimit
            * @return size_type The maximimum number of entity.
            * @brief Return the maximimum number of entity authorized at once.
            */
            size_type getEntityLimit(void) const noexcept
            {
                return this->_sparseArrayDefaultSize;
            }
            /**
            * @fn getActiveEntityCount
            * @return size_type The number of active entities.
            * @brief Return the number of active entities.
            */
            size_type getActiveEntitiesCount(void) const noexcept
            {
                return this->_currentEntitiesCount;
            }
            /**
            * @fn clearEntities
            * @brief Delete all the active entities.
            * THIS WILL INVALIDATE ALL THE REFERENCES TO THE ENTITIES.
            */
            void clearEntities(void)
            {
                for (auto it : this->_currentEntities) {
                    if (it.has_value() == false) {
                        continue;
                    }
                    this->removeEntity(*it);
                }
            }
            /**
            * @fn clearComponents
            * @brief Delete all the component's SparseArray.
            * THIS WILL INVALIDATE ALL THE REFERENCES TO THE ENTITIES.
            */
            void clearComponents(void)
            {
                this->_registry.clear();
            }
            /**
            * @fn isEntityActive
            * @brief check if the entity index is active.
            */
            bool isEntityActive(size_type idx)
            {
                if (this->_currentEntities[idx].has_value() == false) {
                    return false;
                }
                return this->_currentEntities[idx]->isActive();
            }


        private:
            /**
            * @fn updateMaxEntityId
            * @brief Update this->_maxEntityId with the maximimum id in this->_currentEntities.
            * Searchs from 0 to (this->_maxEntityId - 1)
            */
            inline void updateMaxEntityId(void) noexcept
            {
                size_type maxDifferent = 0;

                for (size_type i = 0; i < this->_maxEntityId; i++) {
                    // no value : skip
                    if (this->_currentEntities[i].has_value() == false) {
                        continue;
                    }
                    if (i < maxDifferent) {
                        continue;
                    }
                    maxDifferent = i;
                }
                this->_maxEntityId = maxDifferent;
            }


        private:
            ComponentRegistry _registry;
            #define DEFAULTSPARSEARRAYSIZE 100
            size_type _sparseArrayDefaultSize = DEFAULTSPARSEARRAYSIZE;
            SparseArray<Entity> _currentEntities;
            size_type _maxEntityId = 0;
            size_type _currentEntitiesCount = 0;
            //                      Type    - std::function<void(ECS&, Component&, Entity&)>
            std::unordered_map<std::type_index, std::any> _functionArray;
    };  // class ECS
}  // namespace Rengine
#endif  // SRC_ECS_HPP_
