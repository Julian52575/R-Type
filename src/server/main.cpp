#include "include/server.hpp"

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
            return 84;
        }
        int port = std::stoi(argv[1]);
        Server server(port);
        server.run();
    } catch(const std::exception& e) {
        std::cerr << "[SERVER] Uncatched Error: " << e.what() << '\n';
        return 84;
    }
}
