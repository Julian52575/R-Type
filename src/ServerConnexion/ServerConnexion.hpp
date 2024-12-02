#pragma once
#include <asio.hpp>
#include "tsqueue.hpp"
#include "Messages/OwnedMessage.hpp"
#include <vector>
#define MAX_UDP_BUFF_SIZE 4096

template <typename T>
class ServerConnection {
    private:
        asio::io_context m_context;
        asio::ip::udp::socket m_socket;
        std::thread m_threadContext;
        std::thread m_threadServer;
        tsqueue<OwnedMessage<T>> m_qMessagesIn;
        tsqueue<OwnedMessage<T>> m_qMessagesOut;
        std::vector<User> m_vUsers;

        void accept();
        bool SendAsio(const Message<T> &msg, User recipient);

    public:
        ServerConnection(int32_t port);
        bool Send(const Message<T> &msg, User recipient);
        bool SendAll(const Message<T> &msg);
        std::vector<User> &GetUsers();
        User &GetUser(unsigned int id);
        std::optional<OwnedMessage<T>> Receive();
        ~ServerConnection();
};

template <typename T>
ServerConnection<T>::ServerConnection(int32_t port) : m_socket(m_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
    m_threadContext = std::thread([this]() { m_context.run(); });
    m_threadServer = std::thread([this]() {
        while (m_socket.is_open()) {
            while (!m_qMessagesOut.empty()) {
                std::optional<OwnedMessage<T>> msg = m_qMessagesOut.pop();
                if (!msg)
                    continue;
                SendAsio(msg->GetMessage(), msg->GetOwner());
            }
        }
    });
    accept();
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
                    std::cout << msg << std::endl;
                    if (msg.header.type == T::ServerConnexionRequest) {
                        User user;
                        user.endpoint = *remote_endpoint;
                        user.id = m_vUsers.empty() ? 1 : m_vUsers.back().id + 1;
                        m_vUsers.push_back(user);
                        std::cout << "[SERVER] User connected, ID: " << user.id << std::endl;
                        Message<T> msg2;
                        msg2.header.type = T::ServerConnexionResponse;
                        msg2 << user.id;
                        Send(msg2, user);
                    } else {
                        try {
                            unsigned int id;
                            if (msg.header.size == 0) {
                                throw std::runtime_error("Message has no ID");
                            }
                            msg >> id;
                            User user = GetUser(id);
                            m_qMessagesIn.push(OwnedMessage<T>(user, msg));
                        }
                        catch(const std::runtime_error& e) {
                            std::cerr << e.what() << '\n';
                        }   
                    }
                } catch (const std::exception& e) {
                    std::cerr << "[SERVER] Error deserializing message: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "[SERVER] Error receiving packet: " << ec.message() << std::endl;
            }
            recv_packet_buf->fill(0);
            accept();
        }
    );
}

template <typename T>
bool ServerConnection<T>::SendAsio(const Message<T>& msg, User recipient) {
    std::vector<uint8_t> serialized_msg = msg.serialize();

    m_socket.async_send_to(asio::buffer(serialized_msg), recipient.endpoint, 
        [serialized_msg](std::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "[SERVER] Failed to send message: " << ec.message() << std::endl;
            } else {
                std::cout << "Message sent successfully (" << bytes_transferred << " bytes)" << std::endl;
            }
        });

    return true;
}

template <typename T>
bool ServerConnection<T>::Send(const Message<T> &msg, User recipient) {
    m_qMessagesOut.push(OwnedMessage<T>(recipient, msg));
    return true;
}


template <typename T>
std::vector<User> &ServerConnection<T>::GetUsers() {
    return m_vUsers;
}

template <typename T>
User &ServerConnection<T>::GetUser(unsigned int id) {
    auto it = std::find_if(m_vUsers.begin(), m_vUsers.end(), [id](const User &u) { return u.id == id; });

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
    return false;
}

template <typename T>
std::optional<OwnedMessage<T>> ServerConnection<T>::Receive() {
    return m_qMessagesIn.pop();
}

template <typename T>
ServerConnection<T>::~ServerConnection() {
    m_context.stop();
    if (m_threadContext.joinable()) {
        m_threadContext.join();
    }
    if (m_threadServer.joinable()) {
        m_threadServer.join();
    }
    if (m_socket.is_open()) {
        m_socket.close();
    }
    std::cout << "[SERVER] Server stopped" << std::endl;
}