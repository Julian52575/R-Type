#pragma once
#include <cstdint>

class Entity {
    public:
        explicit Entity(uint32_t id);
        operator uint32_t() const;
        uint32_t getId() const;

    private:
        uint32_t id;
};

