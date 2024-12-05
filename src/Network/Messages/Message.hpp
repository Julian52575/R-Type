/**
 * @file Message.hpp
 * @brief Defines the `Message` and `MessageHeader` structures for handling generic messages.
 */

#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <iomanip>

/**
 * @struct MessageHeader
 * @brief Generic header for a message.
 *
 * @tparam T Type representing the message type.
 */
template <typename T>
struct MessageHeader {
    T type;  ///< The type of the message.
    uint32_t size;  ///< Total size of the message (header + body).
};

/**
 * @struct Message
 * @brief Generic structure for a message containing a header and a body.
 *
 * @tparam T Type representing the message type.
 */
template <typename T>
struct Message {
    MessageHeader<T> header;  ///< The message header.
    std::vector<uint8_t> body;  ///< The message body, containing raw data.

    /**
     * @brief Calculates the total size of the message (header + body).
     * 
     * @return The total size of the message in bytes.
     */
    size_t size() const {
        return sizeof(MessageHeader<T>) + body.size();
    }

    /**
     * @brief Overloads the `<<` operator to print a message.
     *
     * @param os The output stream.
     * @param msg The message to print.
     * @return A reference to the output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Message<T> &msg) {
        os << "Type: " << static_cast<int>(msg.header.type) << " Size: " << msg.header.size;
        return os;
    }

    /**
     * @brief Overloads the `<<` operator to append data to the message body.
     *
     * @tparam DataType Type of the data to append to the message.
     * @param msg The message to append data to.
     * @param data The data to append.
     * @return A reference to the modified message.
     * @throws static_assert If the data type is incompatible.
     */
    template <typename DataType>
    friend Message<T>& operator<<(Message<T>& msg, const DataType& data) {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        size_t i = msg.body.size();
        msg.body.resize(msg.body.size() + sizeof(DataType));

        std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
        msg.header.size = msg.size();
        return msg;
    }

    /**
     * @brief Overloads the `>>` operator to extract data from the message body.
     *
     * @tparam DataType Type of the data to extract.
     * @param msg The message to extract data from.
     * @param data The extracted data.
     * @return A reference to the modified message.
     * @throws static_assert If the data type is incompatible.
     * @throws std::out_of_range If the message body is empty.
     */
    template <typename DataType>
    friend Message<T>& operator>>(Message<T>& msg, DataType& data) {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        if (msg.body.empty()) {
            throw std::out_of_range("No data to extract");
        }
        size_t i = msg.body.size() - sizeof(DataType);
        std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

        msg.body.resize(i);
        msg.header.size = msg.size();
        return msg;
    }

    /**
     * @brief Serializes the message into a byte vector.
     *
     * @return A vector containing the serialized bytes of the message.
     */
    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> buffer;

        buffer.resize(sizeof(MessageHeader<T>));
        std::memcpy(buffer.data(), &header, sizeof(MessageHeader<T>));
        buffer.insert(buffer.end(), body.begin(), body.end());
        return buffer;
    }
};

/**
 * @brief Deserializes a byte vector into a message.
 *
 * @tparam T Type representing the message type.
 * @param data A vector containing the serialized bytes.
 * @return A deserialized message.
 * @throws std::runtime_error If the data size is smaller than the header size.
 */
template <typename T>
Message<T> desirialized(const std::vector<uint8_t>& data) {
    if (data.size() < sizeof(MessageHeader<T>)) {
        throw std::runtime_error("Data size is too small for header");
    }

    Message<T> msg;
    std::memcpy(&msg.header, data.data(), sizeof(MessageHeader<T>));
    if (msg.header.size == 0) {
        return msg;
    }
    msg.body.insert(msg.body.end(), data.begin() + sizeof(MessageHeader<T>), data.end());
    return msg;
}
