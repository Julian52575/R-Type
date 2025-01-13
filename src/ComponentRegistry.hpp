//
// EPITECH PROJECT, 2024
//
// File description:
//
//

#ifndef SRC_COMPONENTREGISTRY_HPP_
#define SRC_COMPONENTREGISTRY_HPP_
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <any>

#include "./SparseArray.hpp"

namespace Rengine {

    class ComponentRegistryExceptionAlreadyRegistred : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::ComponentRegistry: Component already registered."; };
    };
    class ComponentRegistryExceptionNotRegistred : public std::exception {
        public:
            const char *what() const noexcept { return "Rengine::ComponentRegistry: Component not registered."; };
    };
    /**
     * @addtogroup Rengine
     * @namespace Rengine
     * @class ComponentRegistry
     * @brief This class stores everything related to the active components.
    */
    class ComponentRegistry {
        public:
            using size_type = typename SparseArray<int> :: size_type;
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
                auto i = std::type_index(typeid(Component));
                auto it = this->_componentsArrays.find(i);

                if (it != this->_componentsArrays.end()) {
                    throw ComponentRegistryExceptionAlreadyRegistred();
                }
                this->_componentsArrays[i] = SparseArray<Component>();
                return std::any_cast<SparseArray<Component>&>(this->_componentsArrays[i]);
            }
            /**
            * @fn registerComponent
            * @template Component The type to be registered in the registry.
            * @param size The default number of element in the SparseArray.
            * @return SparseArray<Component>& A Rengine::SparseArray of your templated class.
            * @exception std::runtime_error Exception raised when Component is already registered.
            * @brief Create a new SparseArray of the templated type and stores it.
            */
            template <class Component>
            SparseArray<Component>& registerComponent(size_type size)
            {
                auto i = std::type_index(typeid(Component));
                auto it = this->_componentsArrays.find(i);

                if (it != this->_componentsArrays.end()) {
                    throw ComponentRegistryExceptionAlreadyRegistred();
                }
                SparseArray<Component> sp = SparseArray<Component>();

                if (sp.size() < size) {
                    sp.addSize(size - sp.size());
                }
                this->_componentsArrays[i] = std::move(sp);
                return std::any_cast<SparseArray<Component>&>(this->_componentsArrays[i]);
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
                auto i = std::type_index(typeid(Component));
                auto it = this->_componentsArrays.find(i);

                if (it == this->_componentsArrays.end()) {
                    throw ComponentRegistryExceptionNotRegistred();
                }
                return std::any_cast<SparseArray<Component>&>(it->second);
            }
            /**
            * @fn getComponents
            * @template Component The type you want to retrive from the registry.
            * @return SparseArray<Component>& A Rengine::SparseArray of your templated class.
            * @exception std::runtime_error Exception raised when Component is not registered.
            * @brief Retrive the Rengine::SparseArray associated with the templated type.
            */
            template <class Component>
            SparseArray<Component> const& getComponents(void) const
            {
                auto i = std::type_index(typeid(Component));
                auto it = this->_componentsArrays.find(i);

                if (it == this->_componentsArrays.end()) {
                    throw ComponentRegistryExceptionNotRegistred();
                }
                return std::any_cast<SparseArray<Component>&>(it->second);
            }

        private:
            //                      Type    -   SparseArray<Type>
            std::unordered_map<std::type_index, std::any> _componentsArrays;
    };

}  // namespace Rengine

#endif  // SRC_COMPONENTREGISTRY_HPP_
