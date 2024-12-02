#pragma once
#include <string>
#include <asio.hpp>

struct User {
    int32_t id;
    asio::ip::udp::endpoint endpoint;
};
