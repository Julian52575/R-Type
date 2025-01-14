#pragma once

#include <asio.hpp>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <optional>
#include <thread>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include "./tsqueue.hpp"
#include "./Messages/Message.hpp"
#include "./User/User.hpp"
#include "./AConnexion.hpp"
#define MAX_UDP_BUFF_SIZE 1024

/**
 * @brief Represents a server-side connection that manages UDP-based communication with multiple users.
 *
 * @tparam T The type of messages handled by the server.
 */
template <typename T>
class ServerUDP : public AConnexion {
private:
    std::string _serverName;
    /**
     * @brief UDP socket used for sending and receiving data.
     */
    asio::ip::udp::socket m_socket;

    /**
     * @brief Thread-safe queue to store incoming messages.
     */
    tsqueue<std::pair<asio::ip::udp::endpoint, Message<T>>> m_qMessagesIn;

    /**
     * @brief List of users connected to the server.
     */
    std::vector<User> m_vUsers;

    /**
     * @brief Starts accepting incoming packets and processes them.
     */
    void accept();

public:
    /**
     * @brief Constructs a server connection listening on the specified port.
     *
     * @param port The port to bind the server to.
     */
    ServerUDP(int32_t port, std::string serverName);
    std::string getServerName() { return _serverName; }
    uint16_t getPort() { return m_socket.local_endpoint().port(); }

    /**
     * @brief Sends a message to a specific recipient.
     *
     * @param msg The message to send.
     * @param recipient The recipient of the message.
     * @return true if the message was successfully sent.
     * @return false otherwise.
     */
    bool Send(const Message<T> &msg, User recipient);

    /**
     * @brief Sends a message to all connected users.
     *
     * @param msg The message to send.
     * @return true if the message was successfully sent to all users.
     * @return false otherwise.
     */
    bool SendAll(const Message<T> &msg);

    /**
     * @brief Retrieves the list of currently connected users.
     *
     * @return A reference to the vector of connected users.
     */
    std::vector<User> &GetUsers();

    /**
     * @brief Retrieves a specific user by their ID.
     *
     * @param id The ID of the user to retrieve.
     * @return A reference to the user.
     * @throws std::runtime_error if the user with the given ID is not found.
     */
    User &GetUser(asio::ip::udp::endpoint endpoint);

    void AddUser(User &user) {
        m_vUsers.push_back(user);
    }

    void RemoveUser(User &user) {
        m_vUsers.erase(std::remove(m_vUsers.begin(), m_vUsers.end(), user), m_vUsers.end());
    }

    /**
     * @brief Retrieves the next incoming message from the queue.
     *
     * @return An optional containing the next message, or std::nullopt if no messages are available.
     */
    std::optional<std::pair<asio::ip::udp::endpoint, Message<T>>> Receive();

    /**
     * @brief Destructor for the server connection.
     *
     * Stops the ASIO context, joins the thread, and cleans up the socket.
     */
    ~ServerUDP();
};

template <typename T>
ServerUDP<T>::ServerUDP(int32_t port, std::string serverName) : m_socket(this->getContext(), asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
    _serverName = serverName;
    accept();
    this->start();
    std::cout << "[" << _serverName << " - UDP] Server started on: " << m_socket.local_endpoint() << std::endl;
}

template <typename T>
void ServerUDP<T>::accept() {
    auto recv_packet_buf = std::make_shared<std::array<uint8_t, MAX_UDP_BUFF_SIZE>>();
    auto remote_endpoint = std::make_shared<asio::ip::udp::endpoint>();

    m_socket.async_receive_from(asio::buffer(*recv_packet_buf), *remote_endpoint,
        [this, recv_packet_buf, remote_endpoint](std::error_code ec, std::size_t length) {
            if (!ec) {
                try {
                    std::vector<uint8_t> data(recv_packet_buf->begin(), recv_packet_buf->begin() + length);
                    Message<T> msg = desirialized<T>(data);
                    m_qMessagesIn.push({*remote_endpoint, msg});
                } catch (const std::exception &e) {
                    std::cerr << "[" << this->getServerName() << " - UDP] Error deserializing message: " << e.what() << std::endl;
                }
            } else {
                if (ec == asio::error::bad_descriptor && !m_socket.is_open()) {
                    return;
                } else if (ec == asio::error::operation_aborted && !m_socket.is_open()) {
                    return;
                }
                std::cerr << "[" << this->getServerName() << " - UDP] Error receiving packet: " << ec.message() << std::endl;
            }
            recv_packet_buf->fill(0);
            accept();
        });
}

template <typename T>
bool ServerUDP<T>::Send(const Message<T> &msg, User recipient) {
    std::vector<uint8_t> serialized_msg = msg.serialize();

    m_socket.async_send_to(asio::buffer(serialized_msg), recipient.endpoint,
        [this, serialized_msg](std::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "[" << this->getServerName() << " - UDP] Failed to send message: " << ec.message() << std::endl;
            }
        });
    return true;
}

template <typename T>
std::vector<User> &ServerUDP<T>::GetUsers() {
    return m_vUsers;
}

template <typename T>
User &ServerUDP<T>::GetUser(asio::ip::udp::endpoint endpoint) {
    auto it = std::find_if(m_vUsers.begin(), m_vUsers.end(), [endpoint](const User &user) {
        return user.endpoint == endpoint;
    });

    if (it != m_vUsers.end()) {
        return *it;
    } else {
        throw std::runtime_error("User not found");
    }
}

template <typename T>
bool ServerUDP<T>::SendAll(const Message<T> &msg) {
    for (auto &user : m_vUsers) {
        Send(msg, user);
    }
    return true;
}

template <typename T>
std::optional<std::pair<asio::ip::udp::endpoint, Message<T>>> ServerUDP<T>::Receive() {
    if (m_qMessagesIn.empty()) {
        return std::nullopt;
    }
    return m_qMessagesIn.pop();
}

template <typename T>
ServerUDP<T>::~ServerUDP() {
    if (m_socket.is_open()) {
        m_socket.close();
    }
    std::cout << "[" << _serverName << " - UDP] Server stopped" << std::endl;
}
