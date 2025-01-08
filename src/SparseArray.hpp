/*
// EPITECH PROJECT, 2024
//
// File description:
//
*/

#ifndef SRC_SPARSEARRAY_HPP_
#define SRC_SPARSEARRAY_HPP_
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <utility>
#include <iterator>
#include <iostream>
#include <optional>

namespace Rengine {

    /**
     * @addtogroup Rengine
     * @namespace Rengine
     * @class SparseArray
     * @template Component The type to store.
     * @brief A container.
    */
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
            /**
            * @fn SparseArray
            * @brief Create a new instance of SparseArray.
            */
            SparseArray(void) = default;
            /**
            * @fn SparseArray
            * @param size The number of element held by the underlying container.
            * @brief Create a new instance of SparseArray.
            */
            explicit SparseArray(size_type size)
            {
                this->_data.resize(size);
            }
            /**
            * @fn SparseArray
            * @brief The copy constructor.
            */
            explicit SparseArray(SparseArray const &og)
            {
                this->_data = og._data;
            }
            /**
            * @fn SparseArray
            * @brief The move constructor.
            */
            explicit SparseArray(SparseArray &&og) noexcept
            {
                this->_data = std::move(og._data);
            }
            /**
            * @fn ~SparseArray
            * @brief Destruct an instance of SparseArray.
            */
            ~SparseArray(void) = default;
            /**
            * @fn operator=
            * @brief The copy assignment operator.
            */
            SparseArray &operator=(SparseArray const &og)  // copy assignment operator
            {
                if (this != &og) {
                    this->_data = og._data;
                }
                return *this;
            }
            /**
            * @fn operator=
            * @brief The move assignment operator.
            */
            SparseArray &operator=(SparseArray &&og) noexcept
            {
                if (this != &og) {
                    this->_data = std::move(og._data);
                }
                return *this;
            }
            /**
            * @fn operator[]
            * @brief Return a reference to the object stored at position idx.
            */
            reference_type operator[](size_t idx)
            {
                if (idx >= this->_data.size()) {
                    throw std::out_of_range("Index out of range");
                }
                return this->_data[idx];
            }
            /**
            * @fn operator[]
            * @brief Return a const reference to the object stored at position idx.
            */
            const_reference_type operator[](size_t idx) const
            {
                if (idx >= this->_data.size()) {
                    throw std::out_of_range("Index out of range");
                }
                return this->_data[idx];
            }
            /**
            * @fn begin
            * @brief Returns a iterator to the start of the SparseArray.
            */
            iterator begin(void)
            {
                return this->_data.begin();
            }
            /**
            * @fn begin
            * @brief Returns a const iterator to the start of the SparseArray.
            */
            const_iterator begin(void) const
            {
                return this->_data.begin();
            }
            /**
            * @fn begin
            * @brief Returns a const iterator to the start of the SparseArray. (c style)
            */
            const_iterator cbegin(void) const
            {
                return this->_data.cbegin();
            }
            /**
            * @fn begin
            * @brief Returns a iterator one after the end of the SparseArray.
            */
            iterator end(void)
            {
                return this->_data.end();
            }
            /**
            * @fn begin
            * @brief Returns a iterator one after the end of the SparseArray.
            */
            const_iterator end(void) const
            {
                return this->_data.end();
            }
            /**
            * @fn begin
            * @brief Returns a iterator one after the end of the SparseArray.
            */
            const_iterator cend(void) const
            {
                return this->_data.cend();
            }
            /**
            * @fn reset
            * @param idx Index of the element to reset
            * @brief Reset the element at index idx.
            */
            void reset(size_type idx)
            {
                if (idx > this->size()) {
                    return;
                }
                this->_data[idx].reset();
            }
            /**
            * @fn size
            * @brief Returns the number of object stored in the SparseArray.
            */
            size_type size(void) const
            {
                return this->_data.size();
            }
            /**
            * @fn insertAt
            * @param idx Index at which to store the new object.
            * @param con Reference to the new Component to store.
            * @brief Add con at the index idx of the SparseArray.
            * Note: The SparseArray will be resized if idx W this->size()
            */
            reference_type insertAt(size_type idx, Component const &con)
            {
                if (idx >= this->size()) {
                    this->_data.resize(idx + 1);
                }
                // this->_data.insert(pos, con);
                this->_data[idx] = con;
                return this->_data[idx];
            }
            /**
            * @fn insertAt
            * @param idx Index at which to store the new object.
            * @param con Reference to the new Component to store.
            * @brief Add con at the index idx of the SparseArray.
            * Note: The SparseArray will be resized if idx W this->size()
            */
            reference_type insertAt(size_type idx, Component &&con)
            {
                if (idx >= this->size()) {
                    this->_data.resize(idx + 1);
                }
                auto it = std::next(this->_data.begin(), idx);

                this->_data.insert(it, con);
                // this->_data[pos] = std::move(con);
                return this->_data[idx];
            }
            /**
            * @fn emplaceAt
            * @param idx Index at which to store the new object.
            * @param args Parameter pack to unpack to the constructor of the SparseArray templated class.
            * @brief Construct a new object at the index idx of the SparseArray.
            * Note: The SparseArray will be resized if idx W this->size()
            */
            template <class ... Params>
            reference_type emplaceAt( size_type idx, Params &&... args)
            {
                if (idx >= this->size()) {
                    this->_data.resize(idx + 1);
                }
                this->_data[idx].emplace(std::forward<Params>(args)...);
                return this->_data[idx];
            }
            /**
            * @fn erase
            * @exception std::out_of_range pos is > this->size().
            * @brief Call the destructor of the object at pos.
            */
            void erase(size_type pos)
            {
                if (pos >= this->_data.size()) {
                    std::string err = "SparseArray: Index " + std::to_string(pos) + " is out of range";

                    throw std::out_of_range(err);
                }
                this->_data[pos].reset();
            }
            /**
            * @fn getIndex
            * @return i The index of the object con.
            * @return static_cast<size_type>(-1) The object was not found.
            * @brief Get the index of the con object.
            */
            size_type getIndex(value_type const &con) const
            {
                size_type i = 0;

                for (auto it = this->_data.begin(); it != this->_data.end(); it++) {
                    if (std::addressof(*it) == std::addressof(con)) {
                        return i;
                    }
                    i++;
                }
                return static_cast<size_type>(-1);
            }
            /**
            * @fn getIndex
            * @return i The index of the object con.
            * @return (size_type)-1 The object was not found.
            * @brief Get the index of the con object.
            */
            size_type getIndex(Component const &con) const
            {
                for (size_type i = 0; i < this->_data.size(); ++i) {
                    if (this->_data[i].has_value() && this->_data[i].value() == con) {
                        return i;
                    }
                }
                return static_cast<size_type>(-1);
            }
            /**
            * @fn getIndex
            * @param length The number of element to add to the SparseArray.
            * @brief Resize the SparseArray by adding size element to it.
            * Resizing to a smaller size results in a No-Op to avoid glitches.
            * Ie: size 10 + addSize(15) = 25 elements and max index [24]
            */
            void addSize(size_type size)
            {
                size_type oldSize = this->_data.size();
                size_type newSize = this->_data.size() + size;

                if (newSize < oldSize) {
                    return;
                }
                this->_data.resize(this->_data.size() + size);
            }

        private :
            container_t _data;
    };
}  // namespace Rengine

#endif  // SRC_SPARSEARRAY_HPP_
