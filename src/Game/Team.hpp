//
#pragma once

#include <cstdint>

namespace RType {

    enum Team : uint64_t {
        TeamPlayer = 0b0001,
        TeamEnemy = 0b0010
    };

}  // namespace RType
