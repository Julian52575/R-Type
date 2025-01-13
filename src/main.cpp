#include "Lobby/lobby.hpp"

static Lobby *lobby = nullptr;

void signalHandler(int signum) {
    std::cout << "\n[SERVER] Signal " << signum << " received. Stopping the server..." << std::endl;
    lobby->setRunning(false);
};

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
            return 84;
        }
        uint16_t port = std::stoi(argv[1]);
        
        Lobby server(port);
        lobby = &server;
        std::signal(SIGINT, signalHandler);

        server.run();
    } catch(const std::exception& e) {
        std::cerr << "[SERVER] Uncatched Error: " << e.what() << '\n';
        return 84;
    }
}
