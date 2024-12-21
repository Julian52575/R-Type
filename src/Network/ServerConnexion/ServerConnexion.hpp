#pragma once

#include "../tsqueue.hpp"
#include "../Messages/Message.hpp"
#include "../User/User.hpp"

#include <asio.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <optional>
#include <thread>
#include <algorithm>
#include <stdexcept>
#include <utility>
#define MAX_UDP_BUFF_SIZE 4096

/**
 * @brief Represents a server-side connection that manages UDP-based communication with multiple users.
 *
 * @tparam T The type of messages handled by the server.
 */
template <typename T>
class ServerConnection {
private:
    /**
     * @brief ASIO context for managing asynchronous operations.
     */
    asio::io_context m_context;

    /**
     * @brief UDP socket used for sending and receiving data.
     */
    asio::ip::udp::socket m_socket;

    /**
     * @brief Thread running the ASIO context.
     */
    std::thread m_threadContext;

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
    explicit ServerConnection(int32_t port);

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
    ~ServerConnection();
};

template <typename T>
ServerConnection<T>::ServerConnection(int32_t port)
    : m_socket(m_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
    m_threadContext = std::thread([this]() { m_context.run(); });
    accept();
#ifdef DEBUG
    std::cout << "[SERVER] Server started on: " << m_socket.local_endpoint() << std::endl;
#endif
}

template <typename T>
void ServerConnection<T>::accept() {
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
                    std::cerr << "[SERVER] Error deserializing message: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "[SERVER] Error receiving packet: " << ec.message() << std::endl;
            }
            recv_packet_buf->fill(0);
            accept();
        });
}

template <typename T>
bool ServerConnection<T>::Send(const Message<T> &msg, User recipient) {
    std::vector<uint8_t> serialized_msg = msg.serialize();

    m_socket.async_send_to(asio::buffer(serialized_msg), recipient.endpoint,
        [serialized_msg](std::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "[SERVER] Failed to send message: " << ec.message() << std::endl;
            }
        });
    return true;
}

template <typename T>
std::vector<User> &ServerConnection<T>::GetUsers() {
    return m_vUsers;
}

template <typename T>
User &ServerConnection<T>::GetUser(asio::ip::udp::endpoint endpoint) {
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
bool ServerConnection<T>::SendAll(const Message<T> &msg) {
    for (auto &user : m_vUsers) {
        Send(msg, user);
    }
    return true;
}

template <typename T>
std::optional<std::pair<asio::ip::udp::endpoint, Message<T>>> ServerConnection<T>::Receive() {
    return m_qMessagesIn.pop();
}

template <typename T>
ServerConnection<T>::~ServerConnection() {
    m_context.stop();
    if (m_threadContext.joinable()) {
        m_threadContext.join();
    }
    if (m_socket.is_open()) {
        m_socket.close();
    }
#ifdef DEBUG
    std::cout << "[SERVER] Server stopped" << std::endl;
#endif
}

