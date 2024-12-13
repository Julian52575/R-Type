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

inline bool operator==(const User &lhs, const User &rhs) {
    return lhs.endpoint == rhs.endpoint;
}

// Définition du hash pour User
namespace std {
    template <>
    struct hash<User> {
        std::size_t operator()(const User &user) const {
            // Combine le hash de l'adresse IP et du port
            return std::hash<std::string>()(user.endpoint.address().to_string()) ^
                   std::hash<unsigned short>()(user.endpoint.port());
        }
    };
}
