#pragma once
#include <cstdint>

class Entity {
    public:
        explicit Entity(uint32_t id);
        explicit Entity(uint32_t id, uint16_t configId);
        operator uint32_t() const;
        uint32_t getId() const;
        void setId(uint32_t id);
        uint16_t getConfigId() const;
        void setConfigId(uint16_t configId);

    private:
        uint32_t id;
        uint16_t configId;
};

