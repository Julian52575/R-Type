#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <iomanip>

template <typename T>
struct MessageHeader {
    T type;
    uint32_t size;
};

template <typename T>
struct Message {
    MessageHeader<T> header;
    std::vector<uint8_t> body;

    size_t size() const {
        return sizeof(MessageHeader<T>) + body.size();
    }

    friend std::ostream &operator<<(std::ostream &os, const Message<T> &msg) {
        os << "Type: " << static_cast<int>(msg.header.type) << " Size: " << msg.header.size;
        return os;
    }

    template <typename DataType>
    friend Message<T>& operator<<(Message<T>& msg, const DataType& data) {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        size_t i = msg.body.size();
        msg.body.resize(msg.body.size() + sizeof(DataType));

        std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
        msg.header.size = msg.size();
        return msg;
    }

    template <typename DataType>
    friend Message<T>& operator>>(Message<T>& msg, DataType& data) {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        if (msg.body.size() == 0) {
            throw std::out_of_range("No data to extract");
        }
        size_t i = msg.body.size() - sizeof(DataType);
        std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

        msg.body.resize(i);
        msg.header.size = msg.size();
        return msg;
    }

    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> buffer;

        buffer.resize(sizeof(MessageHeader<T>));
        std::memcpy(buffer.data(), &header, sizeof(MessageHeader<T>));
        buffer.insert(buffer.end(), body.begin(), body.end());
        return buffer;
    }
};

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
