//
// EPITECH PROJECT, 2024
//
// File description:
//
//

#ifndef REGISTRY_HPP
#define REGISTRY_HPP
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <typeindex>
#include <any>

#include "Sparse_Array.hpp"

class registry {
    public:
        template <class Component>
        sparse_array<Component>& register_component()
        {
            auto i = std::type_index(typeid(Component));

            this->_components_arrays[i] = sparse_array<Component>();
            return std::any_cast<sparse_array<Component>&>( this->_components_arrays[i] );
        }

        template <class Component>
        sparse_array<Component>& get_components()
        {
            auto i = std::type_index(typeid(Component));
            auto it = this->_components_arrays.find(i);

            if (it == this->_components_arrays.end()) {
                throw std::runtime_error("Component not registered in registry.");
            }
            return std::any_cast<sparse_array<Component>&>(it->second);
        }

        template <class Component>
        sparse_array<Component> const& get_components() const
        {
            auto i = std::type_index(typeid(Component));
            auto it = this->_components_arrays.find(i);

            if (it == this->_components_arrays.end()) {
                throw std::runtime_error("Component not registered in registry.");
            }
            return std::any_cast<sparse_array<Component>&>(it->second);
        }

    private:
        //                      Type    -   sparse_array<Type>
        std::unordered_map<std::type_index, std::any> _components_arrays;
};

#endif
