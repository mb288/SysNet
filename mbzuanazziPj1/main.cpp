#include "tcp_server.hpp"

int main() {
    // Create a TCP server listening on port 60001
    TCPServer server(60001);
    server.start();
    
    return 0;
}
