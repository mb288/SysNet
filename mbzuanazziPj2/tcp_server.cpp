#include "tcp_server.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstdio>
#include<cstdlib>
#include <map>

// Valid port range
#define MIN_PORT 60001
#define MAX_PORT 60099

// Max request size
#define MAX_REQUEST_SIZE 1024

TCPServer::TCPServer(int port) : port(port) {
    if (port < MIN_PORT || port > MAX_PORT) {
        std::cerr << "Error: Port number out of valid range (" << MIN_PORT << " - " << MAX_PORT << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
}

TCPServer::~TCPServer() {
    close(serverSocket);
}
void TCPServer::start() {
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);

    // Create TCP socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse the port
    int opt = 1;
    // Set socket options to reuse the port
if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("Error setting socket options");
    exit(EXIT_FAILURE);
}

    // Bind the socket
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_PENDING_CONNECTIONS) < 0) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;

    while (true) {
        // Accept incoming connection
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
        if (clientSocket < 0) {
            perror("Error accepting connection");
            continue;
        }

        // Handle client request
        handleClientRequest(clientSocket);
    }
}

void TCPServer::sendHttpResponse(int clientSocket, const std::string& response) {
    send(clientSocket, response.c_str(), response.size(), 0);
}

void TCPServer::handleClientRequest(int clientSocket) {
    char buffer[MAX_REQUEST_SIZE];
    int bytesReceived = recv(clientSocket, buffer, MAX_REQUEST_SIZE, 0);
    if (bytesReceived < 0) {
        perror("Error receiving data from client");
        close(clientSocket);
        return;
    }

    // Null-terminate the received data to treat it as a string
    buffer[bytesReceived] = '\0';

    // Parse HTTP request (assuming it's a simple GET request)
    char *requestedFile = strtok(buffer, " ");
    if (requestedFile == nullptr || strcmp(requestedFile, "GET") != 0) {
        // Invalid or unsupported HTTP method
        sendHttpResponse(clientSocket, "HTTP/1.1 400 Bad Request\r\n\r\n");
        close(clientSocket);
        return;
    }

    // Get the requested file path
    requestedFile = strtok(NULL, " ");
    if (requestedFile == nullptr || *requestedFile != '/') {
        // Invalid file path
        sendHttpResponse(clientSocket, "HTTP/1.1 400 Bad Request\r\n\r\n");
        close(clientSocket);
        return;
    }
    // Remove leading slash from the file path
    requestedFile++;

    // Open the requested file
    FILE *file = fopen(requestedFile, "rb");
    if (file == nullptr) {
        // File not found
        sendHttpResponse(clientSocket, "HTTP/1.1 404 Not Found\r\n\r\n");
        close(clientSocket);
        return;
    }

    // Send HTTP header
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    sendHttpResponse(clientSocket, response);

    // Send file content
    char fileBuffer[MAX_REQUEST_SIZE];
    size_t bytesRead;
    while ((bytesRead = fread(fileBuffer, 1, MAX_REQUEST_SIZE, file)) > 0) {
        send(clientSocket, fileBuffer, bytesRead, 0);
    }

    // Close the file and client socket
    fclose(file);
    close(clientSocket);
}
