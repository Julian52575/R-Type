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

#include "./SparseArray.hpp"
#include "./ComponentData.hpp"


namespace Rengine {

    class ComponentRegistry {
        public:
            template <class Component>
            SparseArray<Component>& registerComponent(void)
            {
                auto i = std::type_index(typeid(Component));
                auto it = this->_componentsArrays.find(i);

                if (it != this->_componentsArrays.end()) {
                    throw std::runtime_error("ComponentRegistry: Component already registered.");
                }
                this->_componentsArrays[i] = ComponentData();
                this->_componentsArrays[i]._componentSparseArray = SparseArray<Component>(); 
                return std::any_cast<SparseArray<Component>&>(this->_componentsArrays[i]._componentSparseArray);
            }
            template <class Component>
            SparseArray<Component>& getComponents(void)
            {
                auto i = std::type_index(typeid(Component));
                auto it = this->_componentsArrays.find(i);

                if (it == this->_componentsArrays.end()) {
                    throw std::runtime_error("ComponentRegistry: Component not registered.");
                }
                return std::any_cast<SparseArray<Component>&>(it->second._componentSparseArray);
            }
            template <class Component>
            SparseArray<Component> const& getComponents(void) const
            {
                auto i = std::type_index(typeid(Component));
                auto it = this->_componentsArrays.find(i);

                if (it == this->_componentsArrays.end()) {
                    throw std::runtime_error("ComponentRegistry: Component not registered.");
                }
                return std::any_cast<SparseArray<Component>&>(it->second._componentSparseArray);
            }

        private:
            //                      Type    -   SparseArray<Type>
            std::unordered_map<std::type_index, ComponentData> _componentsArrays;
    };

}  // namespace Rengine

#endif  // SRC_ComponentRegistry_HPP_
