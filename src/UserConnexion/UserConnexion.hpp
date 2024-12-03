/**
 * @file ClientConnection.hpp
 * @brief Header file defining the ClientConnection class for managing UDP communication with a server.
 */

#pragma once

#include "src/tsqueue.hpp"
#include "Messages/Message.hpp"

#include <asio.hpp>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#define MAX_UDP_BUFF_SIZE 4096

/**
 * @brief Represents a client-side connection that manages UDP-based communication with a server.
 *
 * @tparam T The type of messages handled by the client.
 */
template <typename T>
class ClientConnection {
private:
    /**
     * @brief The unique ID assigned to the client by the server.
     */
    unsigned int m_id;

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
     * @brief Remote server's endpoint (IP and port).
     */
    std::shared_ptr<asio::ip::udp::endpoint> m_remote_endpoint;

    /**
     * @brief Thread-safe queue to store incoming messages.
     */
    tsqueue<Message<T>> m_qMessagesIn;

    /**
     * @brief Starts accepting incoming packets from the server and processes them.
     */
    void accept();

public:
    /**
     * @brief Constructs a client connection to a specified server IP and port.
     *
     * @param ip The IP address of the server.
     * @param port The port on which the server is listening.
     */
    ClientConnection(std::string ip, int32_t port);

    /**
     * @brief Sends a message to the server.
     *
     * @param msg The message to send.
     * @return true if the message was successfully sent.
     * @return false otherwise.
     */
    bool Send(Message<T> &msg);

    /**
     * @brief Retrieves the next incoming message from the queue.
     *
     * @return An optional containing the next message, or std::nullopt if no messages are available.
     */
    std::optional<Message<T>> Receive();

    /**
     * @brief Sets the unique ID of the client.
     *
     * @param id The ID assigned to the client by the server.
     */
    void SetId(unsigned int id);

    /**
     * @brief Gets the unique ID of the client.
     *
     * @return The client's ID.
     */
    unsigned int GetId() { return m_id; }

    /**
     * @brief Destructor for the client connection.
     *
     * Stops the ASIO context, joins the thread, and cleans up the socket.
     */
    ~ClientConnection();
};

// Implementation

template <typename T>
ClientConnection<T>::ClientConnection(std::string ip, int32_t port)
    : m_socket(m_context) {
    m_remote_endpoint = std::make_shared<asio::ip::udp::endpoint>(asio::ip::make_address(ip), port);
    m_threadContext = std::thread([this]() { m_context.run(); });
    m_socket.open(asio::ip::udp::v4());
    m_id = 0;
    accept();
    Message<T> msg;
    msg.header.type = T::ServerConnexionRequest;
    msg.header.size = 0;
    Send(msg);
}

template <typename T>
void ClientConnection<T>::accept() {
    auto recv_packet_buf = std::make_shared<std::array<uint8_t, MAX_UDP_BUFF_SIZE>>();

    m_socket.async_receive_from(asio::buffer(*recv_packet_buf), *m_remote_endpoint,
        [this, recv_packet_buf](std::error_code ec, std::size_t length) {
            if (!ec) {
                try {
                    std::vector<uint8_t> data(recv_packet_buf->begin(), recv_packet_buf->begin() + length);
                    Message<T> msg = desirialized<T>(data);
                    std::cout << msg << std::endl;
                    if (msg.header.type == T::ServerConnexionResponse) {
                        msg >> m_id;
                        std::cout << "[CLIENT] New ID: " << m_id << std::endl;
                    } else {
                        m_qMessagesIn.push(msg);
                    }
                } catch (const std::exception& e) {
                    std::cerr << "[CLIENT] Error deserializing message: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "[CLIENT] Error receiving packet: " << ec.message() << std::endl;
            }
            recv_packet_buf->fill(0);
            accept();
        }
    );
}

template <typename T>
bool ClientConnection<T>::Send(Message<T> &msg) {
    msg << m_id;
    std::vector<uint8_t> serialized_msg = msg.serialize();

    m_socket.async_send_to(asio::buffer(serialized_msg), *m_remote_endpoint,
        [serialized_msg](std::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "[CLIENT] Failed to send message: " << ec.message() << std::endl;
            } else {
                std::cout << "Message sent successfully (" << bytes_transferred << " bytes)" << std::endl;
            }
        }
    );
    return true;
}

template <typename T>
std::optional<Message<T>> ClientConnection<T>::Receive() {
    return m_qMessagesIn.pop();
}

template <typename T>
void ClientConnection<T>::SetId(unsigned int id) {
    m_id = id;
}

template <typename T>
ClientConnection<T>::~ClientConnection() {
    m_context.stop();
    if (m_threadContext.joinable()) {
        m_threadContext.join();
    }
    if (m_socket.is_open()) {
        m_socket.close();
    }
    std::cout << "[CLIENT] Server stopped" << std::endl;
}

