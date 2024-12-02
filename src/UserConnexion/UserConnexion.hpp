#pragma once
#include <asio.hpp>
#include "./tsqueue.hpp"
#include "Messages/Message.hpp"
#include <vector>
#include <memory>
#include <string>
#define MAX_UDP_BUFF_SIZE 4096

template <typename T>
class ClientConnection {
    private:
        unsigned int m_id;
        asio::io_context m_context;
        asio::ip::udp::socket m_socket;
        std::thread m_threadContext;
        std::shared_ptr<asio::ip::udp::endpoint> m_remote_endpoint;
        tsqueue<Message<T>> m_qMessagesIn;

        void accept();

    public:
        ClientConnection(std::string ip, int32_t port);
        bool Send(Message<T> &msg);
        std::optional<Message<T>> Receive();
        void SetId(unsigned int id);
        unsigned int GetId() { return m_id; }
        ~ClientConnection();
};

template <typename T>
ClientConnection<T>::ClientConnection(std::string ip, int32_t port) : m_socket(m_context) {
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
    std::cout << "[SERVER] Server stopped" << std::endl;
}
