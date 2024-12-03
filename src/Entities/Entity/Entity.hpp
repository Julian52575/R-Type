#pragma once
#include <cstdint>

class Entity {
    public:
        explicit Entity(std::size_t id);
        operator std::size_t() const;
        std::size_t getId() const;

    private:
        std::size_t id;
};

