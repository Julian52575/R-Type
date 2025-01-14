#pragma once

#include <asio.hpp>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <optional>
#include <thread>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include "./tsqueue.hpp"
#include "./Messages/Message.hpp"
#include "./AConnexion.hpp"
#define MAX_TCP_BUFF_SIZE 1024

template <typename T>
class Connexion {
    private:
        asio::ip::tcp::socket clientSocket;
        Message<T> _msg;

    public:
        explicit Connexion(asio::ip::tcp::socket&& socket);
        asio::ip::tcp::socket& getSocket() { return clientSocket; }
        Message<T>& getMessage() { return _msg; }
        ~Connexion();
};

template <typename T>
Connexion<T>::Connexion(asio::ip::tcp::socket&& socket) : clientSocket(std::move(socket)) {
}

template <typename T>
Connexion<T>::~Connexion() {
}

template <typename T>
class ServerTCP : public AConnexion {
    private:
        std::string _serverName;
        bool _needDeleteList;
        asio::ip::tcp::acceptor _acceptor;
        tsqueue<std::pair<std::shared_ptr<Connexion<T>>, Message<T>>> _incomingMessages;
        std::vector<std::shared_ptr<Connexion<T>>> _clients;
        std::vector<std::shared_ptr<Connexion<T>>> _disconnectedClients;
        bool _eraseClientFromList(std::shared_ptr<Connexion<T>> client);

        void acceptConnexion();
        void readHeader(std::shared_ptr<Connexion<T>> const &client);
        void readBody(std::shared_ptr<Connexion<T>> const &client);

    public:
        ServerTCP(uint16_t port, std::string serverName);
        ServerTCP(uint16_t port, std::string serverName, bool needDeleteList);
        std::string getIp() { return _acceptor.local_endpoint().address().to_string(); }
        uint16_t getPort() { return _acceptor.local_endpoint().port(); }
        ~ServerTCP();
        std::vector<std::shared_ptr<Connexion<T>>> &getDisconnectedClients() { return _disconnectedClients; }
        std::string getServerName() { return _serverName; }

        bool Send(Message<T>& msg, std::shared_ptr<Connexion<T>> recipient);
        bool SendAll(Message<T>& msg);
        bool SendAll(Message<T>& msg, std::shared_ptr<Connexion<T>> except);
        std::optional<std::pair<std::shared_ptr<Connexion<T>>, Message<T>>> Receive();
};

template <typename T>
ServerTCP<T>::ServerTCP(uint16_t port, std::string serverName) : _acceptor(this->getContext(), asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
    _serverName = serverName;
    _needDeleteList = true;
    acceptConnexion();
    this->start();
    std::cout << "[" << _serverName << " - TCP] Server started on: " << _acceptor.local_endpoint() << std::endl;
}

template <typename T>
ServerTCP<T>::ServerTCP(uint16_t port, std::string serverName, bool needDeleteList) : _acceptor(this->getContext(), asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
    _serverName = serverName;
    _needDeleteList = needDeleteList;
    acceptConnexion();
    this->start();
    std::cout << "[" << _serverName << " - TCP] Server started on: " << _acceptor.local_endpoint() << std::endl;
}

template <typename T>
bool ServerTCP<T>::_eraseClientFromList(std::shared_ptr<Connexion<T>> client) {
    auto it = std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end()) {
        _clients.erase(it);
        if (_needDeleteList) {
            if (std::find(_disconnectedClients.begin(), _disconnectedClients.end(), client) == _disconnectedClients.end()) {
                _disconnectedClients.push_back(client);
            }
        }
        return true;
    }
    return false;
}

template <typename T>
void ServerTCP<T>::readHeader(std::shared_ptr<Connexion<T>> const &client) {
    asio::async_read(client->getSocket(), asio::buffer(&client->getMessage().header, sizeof(MessageHeader<T>)),
        [this, client](std::error_code ec, std::size_t length) {
            if (!ec) {
                if (client->getMessage().header.size > 0) {
                    readBody(client);
                } else {
                    Message<T> real_msg;

                    real_msg.header = client->getMessage().header;
                    real_msg.body = client->getMessage().body;
                    _incomingMessages.push(std::make_pair(client, real_msg));
                    client->getMessage() = Message<T>();
                    readHeader(client);
                }
            } else {
                if (ec == asio::error::eof) {
                    std::cout << "[" << this->getServerName() << " - TCP] Client disconnected: " << client->getSocket().remote_endpoint() << std::endl;
                    if (client->getSocket().is_open())
                        client->getSocket().close();
                    _eraseClientFromList(client);
                    return;
                } else if (ec == asio::error::bad_descriptor) {
                    std::cerr << "[" << this->getServerName() << " - TCP] Client disconnected: " << client->getSocket().remote_endpoint() << std::endl;
                    if (client->getSocket().is_open())
                        client->getSocket().close();
                    _eraseClientFromList(client);
                    return;
                } else if (ec == asio::error::operation_aborted) {
                    if (!client->getSocket().is_open())
                        client->getSocket().close();
                    _eraseClientFromList(client);
                    return;
                } else {
                    std::cerr << "[" << this->getServerName() << " - TCP] Error reading header: " << ec.message() << std::endl;
                }
            }
        }
    );
}

template <typename T>
void ServerTCP<T>::readBody(std::shared_ptr<Connexion<T>> const &client) {
    client->getMessage().body.resize(client->getMessage().header.size);
    asio::async_read(client->getSocket(), asio::buffer(client->getMessage().body.data(), client->getMessage().header.size),
        [this, client](std::error_code ec, std::size_t length) {
            if (!ec) {
                _incomingMessages.push(std::make_pair(client, client->getMessage()));
                readHeader(client);
            } else {
                std::cerr << "[" << this->getServerName() << " - TCP] Error reading body: " << ec.message() << std::endl;
            }
        }
    );
}

template <typename T>
void ServerTCP<T>::acceptConnexion() {
    _acceptor.async_accept(
        [this](std::error_code ec, asio::ip::tcp::socket clientSocket){
            if (!ec) {
                auto new_client = std::make_shared<Connexion<T>>(std::move(clientSocket));
                std::cout << "[" << this->getServerName() << " - TCP] New client connected: " << new_client->getSocket().remote_endpoint() << std::endl;
                _clients.push_back(new_client);
                readHeader(new_client);
            } else {
                if (ec == asio::error::bad_descriptor && !_acceptor.is_open()) {
                    return;
                } else if (ec == asio::error::operation_aborted) {
                    if (!_acceptor.is_open())
                        return;
                } else {
                    std::cerr << "[" << this->getServerName() << " - TCP] Error accepting connection: " << ec.message() << std::endl;
                }
            }
            acceptConnexion();
        }
    );
}

template <typename T>
bool ServerTCP<T>::Send(Message<T> &msg, std::shared_ptr<Connexion<T>> recipient) {
    auto serialized_msg = std::make_shared<std::vector<uint8_t>>(msg.serialize());

    if (recipient->getSocket().is_open()) {
        asio::async_write(recipient->getSocket(), asio::buffer(*serialized_msg),
            [this, serialized_msg](std::error_code ec, std::size_t bytes_transferred) {
                if (ec) {
                    std::cerr << "[" << this->getServerName() << " - TCP] Failed to send message: " << ec.message() << std::endl;
                }
            }
        );
        return true;
    }
    return false;
}

template <typename T>
bool ServerTCP<T>::SendAll(Message<T> &msg) {
    bool hasDisconnectedClient = false;
    for (std::shared_ptr<Connexion<T>> client : _clients) {
        if (client->getSocket().is_open()) {
            Send(msg, client);
        } else {
            hasDisconnectedClient = true;
        }
    }
    if (hasDisconnectedClient) {
        for (std::shared_ptr<Connexion<T>> client : _clients) {
            if (!client->getSocket().is_open()) {
                _eraseClientFromList(client);
            }
        }
    }
    return true;
}

template <typename T>
bool ServerTCP<T>::SendAll(Message<T> &msg, std::shared_ptr<Connexion<T>> except) {
    bool hasDisconnectedClient = false;

    for (std::shared_ptr<Connexion<T>> client : _clients) {
        if (client->getSocket().is_open()) {
            if (client == except)
                continue;
            Send(msg, client);
        } else {
            hasDisconnectedClient = true;
        }
    }

    if (hasDisconnectedClient) {
        for (std::shared_ptr<Connexion<T>> client : _clients) {
            if (!client->getSocket().is_open()) {
                _eraseClientFromList(client);
            }
        }
    }

    return true;
}

template <typename T>
std::optional<std::pair<std::shared_ptr<Connexion<T>>, Message<T>>> ServerTCP<T>::Receive() {
    if (_incomingMessages.empty()) {
        return std::nullopt;
    }
    return _incomingMessages.pop();
}

template <typename T>
ServerTCP<T>::~ServerTCP() {
    if (_acceptor.is_open()) {
        _acceptor.close();
    }
    for (std::shared_ptr<Connexion<T>> client : _clients) {
        if (client->getSocket().is_open()) {
            client->getSocket().close();
        }
    }
    std::cout << "[" << _serverName << " - TCP] Server stopped" << std::endl;
}
