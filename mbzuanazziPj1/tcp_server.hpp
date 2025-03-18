#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <string>

class TCPServer {
public:
    TCPServer(int port);
    ~TCPServer();

    void start();
    
private:
    int serverSocket;
    int port;
    static const int MAX_PENDING_CONNECTIONS = 5;
    static const int MAX_REQUEST_SIZE = 1024;

    void sendHttpResponse(int clientSocket, const std::string& response);
    void handleClientRequest(int clientSocket);
};

#endif /* TCP_SERVER_HPP */
