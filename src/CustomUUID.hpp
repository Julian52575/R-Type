#pragma once
#include "Rng/Rng.hpp"
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>

typedef unsigned char uuid_t[16];

namespace Rengine {
    inline void generateUUID(uuid_t& outUuid) {
        for (int i = 0; i < 16; ++i) {
            outUuid[i] = static_cast<unsigned char>(Rengine::RNG::rngFunction() & 0xFF);
        }

        outUuid[6] = (outUuid[6] & 0x0F) | 0x40;
        outUuid[8] = (outUuid[8] & 0x3F) | 0x80;
    }

    inline bool compareUUID(const uuid_t& uuid1, const uuid_t& uuid2) {
        return std::memcmp(uuid1, uuid2, 16) == 0;
    }

    inline std::string printUUID(const uuid_t& uuid) {
        std::stringstream ss;
        for (int i = 0; i < 16; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(uuid[i]);
            if (i == 3 || i == 5 || i == 7 || i == 9) {
                ss << '-';
            }
        }
        return ss.str();
    }

    inline void copyUUID(uuid_t& dest, const uuid_t& src) {
        std::memcpy(dest, src, 16);
    }

    inline std::ostream& operator<<(std::ostream& os, const uuid_t& uuid) {
        std::stringstream ss;
        for (int i = 0; i < 16; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(uuid[i]);
            if (i == 3 || i == 5 || i == 7 || i == 9) {
                ss << '-';
            }
        }
        os << ss.str();
        return os;
    }
}
