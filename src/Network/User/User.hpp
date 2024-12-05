/**
 * @file User.h
 * @brief Declaration of the User structure to manage users and their network endpoints.
 */

#pragma once

#include <string>
#include <asio.hpp>

/**
 * @struct User
 * @brief Structure representing a user with an identifier and a network endpoint.
 */
struct User {
    /**
     * @brief UDP endpoint associated with the user, used for network communication.
     */
    asio::ip::udp::endpoint endpoint;
};

