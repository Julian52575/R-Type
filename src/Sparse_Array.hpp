/*
// EPITECH PROJECT, 2024
//
// File description:
//
*/

#ifndef SPARSE_ARRAY_HPP
#define SPARSE_ARRAY_HPP
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <utility>
#include <iterator>
#include <iostream>
#include <optional>

template <typename Component>
class sparse_array {
    public :
        using value_type = std::optional<Component>;
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std::vector <value_type>; // optionally add your allocator template here .
        using size_type = typename container_t :: size_type ;
        using iterator = typename container_t :: iterator ;
        using const_iterator = typename container_t :: const_iterator ;

    public :
        sparse_array() = default; // You can add more constructors .

        sparse_array(sparse_array const &og)// copy constructor
        {
            _data = og._data;
            // if (og.size() > this->size()) {
            //     this->_data.resize(og.size());
            // }

            // for (size_type i = 0; i < og.size(); i++) {
            //     auto &tmp = og[i];

            //     if (tmp.has_value()) {
            //         this->_data[i].emplace( og[i].value() );
            //     }
            // }
        }
        sparse_array(sparse_array &&og) noexcept // move constructor
        {
            _data = std::move(og._data);
            // if (og.size() > this->size()) {
            //     this->_data.resize(og.size());
            // }

            // for (size_type i = 0; i < og.size(); i++) {
            //     auto &tmp = og[i];

            //     if (tmp.has_value()) {
            //         this->_data[i].emplace( std::move(og[i].value()) );
            //     }
            // }
        }

        ~sparse_array() = default;

        sparse_array &operator=(sparse_array const &og) // copy assignment operator
        {
            if (this != &og) {
                this->_data = og._data;
            }
            return *this;
        }
        sparse_array &operator=(sparse_array &&og) noexcept // move assignment operator
        {
            if (this != &og) {
                this->_data = std::move(og._data);
            }
            return *this;
        }

        reference_type operator[](size_t idx)
        {
            if (idx >= this->_data.size()) {
                throw std::out_of_range("Index out of range");
            }
            return this->_data[idx];
        }
        const_reference_type operator[](size_t idx) const
        {
            if (idx >= this->_data.size()) {
                throw std::out_of_range("Index out of range");
            }
            return this->_data[idx];
        }

        iterator begin()
        {
            return this->_data.begin();
        }
        const_iterator begin() const
        {
            return this->_data.begin();
        }
        const_iterator cbegin() const
        {
            return this->_data.cbegin();
        }

        iterator end()
        {
            return this->_data.end();
        }
        const_iterator end() const
        {
            return this->_data.end();
        }
        const_iterator cend() const
        {
            return this->_data.cend();
        }

        size_type size() const
        {
            return this->_data.size();
        }

        reference_type insert_at(size_type pos, Component const &con)
        {
            if (pos >= this->_data.size()) {
                this->_data.resize(pos + 1);
            }
            // this->_data.insert(pos, con);
            this->_data[pos] = con;
            return this->_data[pos];
        }
        reference_type insert_at(size_type pos, Component &&con)
        {
            if (pos >= this->_data.size()) {
                this->_data.resize(pos + 1);
            }
            // auto it = std::next(this->_data.begin(), pos);

            // this->_data.erase(it);
            // this->_data.insert(it, con);
            this->_data[pos] = std::move(con);
            return this->_data[pos];
        }

        template <class ... Params>
        reference_type emplace_at( size_type pos, Params &&... args)
        {
            if (pos >= this->_data.size()) {
                // this->_data.reserve(pos + 1);
                this->_data.resize(pos + 1);
            }

            // leaving work to emplace
            //this->_data.emplace(it, std::forward<Params>(args)...);

            // allocator_traits with [] operator
            //this->_data[pos] = std::allocator_traits<Params>( std::forward<Params>(args...) );

            // gpt answer
            /* Removed for std::optional
            auto
            //std::allocator<Component>&
                allocator = this->_data.get_allocator();

            // detruit la classe présente a l'index POS du vecteur
            std::allocator_traits<decltype(allocator)>::destroy(allocator, &(this->_data[pos]));
            // construit dans le vecteur
            std::allocator_traits<decltype(allocator)>::construct(allocator, &(this->_data[pos]), std::forward<Params>(args)...);
            */
            this->_data[pos].emplace(std::forward<Params>(args)...);
        // #warning std::optional<T> a la même taille en mémoire si value ou nothing ?
            return this->_data[pos];
        }

        void erase( size_type pos)
        {
            if (pos >= this->_data.size()) {
                throw std::out_of_range("Index out of range");
            }
            // auto it = std::next(this->_data.begin(), pos);
            // this->_data.erase(it);
            this->_data[pos].reset();
        }

        size_type get_index ( value_type const &con ) const
        {
            size_type i = 0;

            for (auto it = this->_data.begin(); it != this->_data.end(); it++) {
                if (std::addressof(*it) == std::addressof(con)) {
                    return i;
                }
                i++;
            }
            return (size_type) -1;
        }

        size_type get_index ( Component const &con ) const
        {
            for (size_type i = 0; i < this->_data.size(); ++i) {
                if (this->_data[i].has_value() && this->_data[i].value() == con) {
                    return i;
                }
            }
            return static_cast<size_type>(-1);
        }

#ifdef DEBUG
        void dump(void) const
        {
            size_t i = 0;

            for (auto &e : this->_data) {
                if (e.has_value()) {
                    std::cout << i << ':' << '\t' << e.value();
                } else {
                    std::cout << i << ":" << '\t' << "NA" << std::endl;
                }
                i++;
            }
        }
#endif

    private :
        container_t _data;
};

#endif
