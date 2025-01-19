#pragma once
#include <string>
#include <memory>
#include <vector>
#include "AConnexion.hpp"
#include "tsqueue.hpp"
#include "Messages/Message.hpp"
#define MAX_UDP_BUFF_SIZE 1024

template <typename T>
class ClientTCP : public AConnexion {
    private:
        asio::ip::tcp::socket _clientSocket;
        tsqueue<Message<T>> _incomingMessages;

    public:
        ClientTCP(std::string serverHost, uint16_t serverPort);
        ~ClientTCP();
        asio::ip::tcp::endpoint getLocalEndpoint() {
            return _clientSocket.local_endpoint();
        }

        void accept();
        bool Send(Message<T> &msg);
        std::optional<Message<T>> Receive();
};

template <typename T>
ClientTCP<T>::ClientTCP(std::string serverHost, uint16_t serverPort) : _clientSocket(this->getContext()) {
    asio::ip::tcp::resolver resolver(this->getContext());
    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(serverHost, std::to_string(serverPort));

    asio::connect(_clientSocket, endpoints);
    accept();
    this->start();
}

template <typename T>
void ClientTCP<T>::accept() {
    auto recv_packet_buf = std::make_shared<std::array<uint8_t, MAX_UDP_BUFF_SIZE>>();

    _clientSocket.async_receive(asio::buffer(*recv_packet_buf),
        [this, recv_packet_buf](std::error_code ec, std::size_t length) {
            if (!ec) {
                try {
                    std::vector<uint8_t> data(recv_packet_buf->begin(), recv_packet_buf->begin() + length);
                    Message<T> msg = desirialized<T>(data);
                    _incomingMessages.push(msg);
                } catch (const std::exception& e) {
                    std::cerr << "[CLIENT - TCP] Error deserializing message: " << e.what() << std::endl;
                }
            } else {
                if (ec == asio::error::eof) {
                    std::cout << "[CLIENT - TCP] Connection closed by server" << std::endl;
                } else if (ec == asio::error::bad_descriptor && !_clientSocket.is_open()) {
                    return;
                } else {
                    std::cerr << "[CLIENT - TCP] Error receiving packet: " << ec.message() << std::endl;
                }
            }
            recv_packet_buf->fill(0);
            accept();
        }
    );
}

template <typename T>
bool ClientTCP<T>::Send(Message<T> &msg) {
    std::vector<uint8_t> serialized_msg = msg.serialize();

    _clientSocket.async_send(asio::buffer(serialized_msg),
        [serialized_msg](std::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "[CLIENT - TCP] Failed to send message: " << ec.message() << std::endl;
            }
        }
    );
    return true;
}

template <typename T>
std::optional<Message<T>> ClientTCP<T>::Receive() {
    return _incomingMessages.pop();
}

template <typename T>
ClientTCP<T>::~ClientTCP() {
    if (_clientSocket.is_open()) {
        _clientSocket.close();
    }
    std::cout << "[CLIENT - TCP] Client stopped" << std::endl;
}
