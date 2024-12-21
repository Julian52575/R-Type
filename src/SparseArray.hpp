/*
// EPITECH PROJECT, 2024
//
// File description:
//
*/

#ifndef SRC_SparseArray_HPP_
#define SRC_SparseArray_HPP_
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <utility>
#include <iterator>
#include <iostream>
#include <optional>

namespace Rengine {

    template <typename Component>
    class SparseArray {
        public:
            using value_type = std::optional<Component>;
            using reference_type = value_type &;
            using const_reference_type = value_type const &;
            using container_t = std::vector <value_type>;  // optionally add your allocator template here .
            using size_type = typename container_t :: size_type;
            using iterator = typename container_t :: iterator;
            using const_iterator = typename container_t :: const_iterator;

        public:
            SparseArray(void) = default;  // You can add more constructors .

            SparseArray(SparseArray const &og)  // copy constructor
            {
                this->_data = og._data;
            }

            SparseArray(SparseArray &&og) noexcept  // move constructor
            {
                this->_data = std::move(og._data);
            }

            ~SparseArray(void) = default;

            SparseArray &operator=(SparseArray const &og)  // copy assignment operator
            {
                if (this != &og) {
                    this->_data = og._data;
                }
                return *this;
            }
            SparseArray &operator=(SparseArray &&og) noexcept  // move assignment operator
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

            iterator begin(void)
            {
                return this->_data.begin();
            }
            const_iterator begin(void) const
            {
                return this->_data.begin();
            }
            const_iterator cbegin(void) const
            {
                return this->_data.cbegin();
            }

            iterator end(void)
            {
                return this->_data.end();
            }
            const_iterator end(void) const
            {
                return this->_data.end();
            }
            const_iterator cend(void) const
            {
                return this->_data.cend();
            }

            size_type size(void) const
            {
                return this->_data.size();
            }

            reference_type insertAt(size_type pos, Component const &con)
            {
                if (pos >= this->_data.size()) {
                    this->_data.resize(pos + 1);
                }
                // this->_data.insert(pos, con);
                this->_data[pos] = con;
                return this->_data[pos];
            }
            reference_type insertAt(size_type pos, Component &&con)
            {
                if (pos >= this->_data.size()) {
                    this->_data.resize(pos + 1);
                }
                auto it = std::next(this->_data.begin(), pos);

                this->_data.insert(it, con);
                //this->_data[pos] = std::move(con);
                return this->_data[pos];
            }

            template <class ... Params>
            reference_type emplaceAt( size_type pos, Params &&... args)
            {
                if (pos >= this->_data.size()) {
                    this->_data.resize(pos + 1);
                }
                this->_data[pos].emplace(std::forward<Params>(args)...);
                return this->_data[pos];
            }

            void erase(size_type pos)
            {
                if (pos >= this->_data.size()) {
                    throw std::out_of_range("SparseArray: Index " + pos + " out of range");
                }
                this->_data[pos].reset();
            }

            size_type getIndex(value_type const &con) const
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

            size_type getIndex(Component const &con) const
            {
                for (size_type i = 0; i < this->_data.size(); ++i) {
                    if (this->_data[i].has_value() && this->_data[i].value() == con) {
                        return i;
                    }
                }
                return static_cast<size_type>(-1);
            }

        private :
            container_t _data;
    };
}  // namespace Rengine

#endif  // SRC_SparseArray_HPP_
