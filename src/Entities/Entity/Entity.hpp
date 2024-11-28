#pragma once
#include <cstdint>

class Entity {
    private :
        std::size_t id;

    public:
        explicit Entity(std::size_t id);
        operator std::size_t() const;
        std::size_t getId() const ;
};
