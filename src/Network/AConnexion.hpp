#pragma once
#include <thread>
#include <asio.hpp>
#include <iostream>
#include <string>

class AConnexion {
    private:
        std::thread _thread;
        asio::io_context _context;

    public:
        AConnexion() {
        }

        std::array<uint16_t, 4> IpToInt(const std::string& ip) {
            std::array<uint16_t, 4> ipInt;
            std::istringstream ss(ip);
            std::string ipPart;
            int i = 0;

            while (std::getline(ss, ipPart, '.') && i < 4) {
                ipInt[i] = std::stoi(ipPart);
                i++;
            }

            if (i != 4) {
                throw std::invalid_argument("Invalid IP address format: " + ip);
            }

            return ipInt;
        }

        std::string IntToIp(const std::array<uint16_t, 4>& ipInt) {
            std::ostringstream ss;

            for (size_t i = 0; i < ipInt.size(); ++i) {
                ss << ipInt[i];
                if (i < ipInt.size() - 1) {
                    ss << '.';
                }
            }

            return ss.str();
        }

        void start() {
            _thread = std::thread([this]() {
                _context.run();
            });
        }

        asio::io_context &getContext() {
            return _context;
        }

        ~AConnexion() {
            if (_thread.joinable()) {
                _thread.join();
            }
            _context.stop();
        }
};
