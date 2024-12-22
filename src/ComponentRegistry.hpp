//
// EPITECH PROJECT, 2024
//
// File description:
//
//

#ifndef SRC_ComponentRegistry_HPP_
#define SRC_ComponentRegistry_HPP_
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

    /**
     * @addtogroup Rengine
     * @namespace Rengine
     * @class ComponentRegistry
     * @brief This class stores everything related to the active components.
    */
    class ComponentRegistry {
        public:
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
                    throw std::runtime_error("ComponentRegistry: Component already registered.");
                }
                this->_componentsArrays[i] = SparseArray<Component>();
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
                    throw std::runtime_error("ComponentRegistry: Component not registered.");
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
                    throw std::runtime_error("ComponentRegistry: Component not registered.");
                }
                return std::any_cast<SparseArray<Component>&>(it->second);
            }

        private:
            //                      Type    -   SparseArray<Type>
            std::unordered_map<std::type_index, std::any> _componentsArrays;
            //                      Type    - std::function<ComponentRegistry&, Type&, SparseArray::size_t>
            std::unordered_map<std::type_index, std::any> _functionArrays;
    };

}  // namespace Rengine

#endif  // SRC_ComponentRegistry_HPP_
