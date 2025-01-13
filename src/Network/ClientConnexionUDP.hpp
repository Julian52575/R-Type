#pragma once
#include "AConnexion.hpp"
#include <string>
#include <memory>
#include <vector>
#include "tsqueue.hpp"
#include "Messages/Message.hpp"
#define MAX_UDP_BUFF_SIZE 1024

template <typename T>
class ClientUDP : public AConnexion {
    private:
        asio::ip::udp::socket _clientSocket;
        tsqueue<Message<T>> _incomingMessages;
        std::shared_ptr<asio::ip::udp::endpoint> _serverEndpoint;
        void accept();

    public:
        ClientUDP(std::string serverHost, uint16_t serverPort);
        ~ClientUDP();

        asio::ip::udp::endpoint getLocalEndpoint() {
            return _clientSocket.local_endpoint();
        }
        bool Send(Message<T> &msg);
        std::optional<Message<T>> Receive();
};

template <typename T>
ClientUDP<T>::ClientUDP(std::string serverHost, uint16_t serverPort) : _clientSocket(getContext()) {
    _serverEndpoint = std::make_shared<asio::ip::udp::endpoint>(asio::ip::make_address(serverHost), serverPort);

    _clientSocket.open(asio::ip::udp::v4());

    _clientSocket.bind(asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));

    auto local_endpoint = _clientSocket.local_endpoint();
    std::cout << "Client bound to local endpoint: " << local_endpoint.address().to_string() << ":" << local_endpoint.port() << std::endl;

    accept();
    this->start();
}

template <typename T>
void ClientUDP<T>::accept() {
    auto recv_packet_buf = std::make_shared<std::array<uint8_t, MAX_UDP_BUFF_SIZE>>();

    _clientSocket.async_receive_from(asio::buffer(*recv_packet_buf), *_serverEndpoint,
        [this, recv_packet_buf](std::error_code ec, std::size_t length) {
            if (!ec) {
                try {
                    std::vector<uint8_t> data(recv_packet_buf->begin(), recv_packet_buf->begin() + length);
                    Message<T> msg = desirialized<T>(data);
                    _incomingMessages.push(msg);
                } catch (const std::exception& e) {
                    std::cerr << "[CLIENT - UDP] Error deserializing message: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "[CLIENT - UDP] Error receiving packet: " << ec.message() << std::endl;
            }
            recv_packet_buf->fill(0);
            accept();
        }
    );
}

template <typename T>
bool ClientUDP<T>::Send(Message<T> &msg) {
    std::vector<uint8_t> serialized_msg = msg.serialize();

    _clientSocket.async_send_to(asio::buffer(serialized_msg), *_serverEndpoint,
        [serialized_msg](std::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "[CLIENT - UDP] Failed to send message: " << ec.message() << std::endl;
            }
        }
    );
    return true;
}

template <typename T>
std::optional<Message<T>> ClientUDP<T>::Receive() {
    return _incomingMessages.pop();
}

template <typename T>
ClientUDP<T>::~ClientUDP() {
    if (_clientSocket.is_open()) {
        _clientSocket.close();
    }
    std::cout << "[CLIENT - UDP] Server stopped" << std::endl;
}
