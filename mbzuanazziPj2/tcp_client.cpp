#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int MAX_REQUEST_SIZE = 1024;

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments are provided
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " (address of server) (port you should connect to) (name of the file being requested)" << std::endl;
        return EXIT_FAILURE;
    }

    // Extract command-line arguments
    std::string serverAddress = argv[1];
    int port = std::stoi(argv[2]);
    std::string fileName = argv[3];

    // Creating the TCP socket
    int tcp_client_socket; // Socket descriptor
    tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0); // Calling the socket function - args: socket domain, socket stream type, TCP protocol (default)

    if (tcp_client_socket < 0) {
        perror("Error creating socket");
        return EXIT_FAILURE;
    }

    // Specify address and port of the remote socket
    struct sockaddr_in tcp_server_address; // Declaring a structure for the address
    tcp_server_address.sin_family = AF_INET; // Structure Fields' definition: Sets the address family of the address the client would connect to
    tcp_server_address.sin_port = htons(port); // Specify and pass the port number to connect - converting in right network byte order
    tcp_server_address.sin_addr.s_addr = inet_addr(serverAddress.c_str()); // Connecting to specified server address

    // Connecting to the remote socket
    if (connect(tcp_client_socket, reinterpret_cast<struct sockaddr*>(&tcp_server_address), sizeof(tcp_server_address)) < 0) {
        perror("Error connecting to server");
        close(tcp_client_socket);
        return EXIT_FAILURE;
    }

    // Construct the HTTP request
    std::string httpRequest = "GET /" + fileName + " HTTP/1.1\r\n";
    httpRequest += "Host: " + serverAddress + "\r\n";
    httpRequest += "Connection: close\r\n\r\n";

    // Send the HTTP request
    if (send(tcp_client_socket, httpRequest.c_str(), httpRequest.length(), 0) < 0) {
        perror("Error sending request");
        close(tcp_client_socket);
        return EXIT_FAILURE;
    }

    // Receive and display server response
    char responseBuffer[MAX_REQUEST_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = recv(tcp_client_socket, responseBuffer, sizeof(responseBuffer), 0)) > 0) {
        std::cout.write(responseBuffer, bytesRead);
    }

    if (bytesRead < 0) {
        perror("Error receiving response");
        close(tcp_client_socket);
        return EXIT_FAILURE;
    }

    // Close socket
    close(tcp_client_socket);

    return 0;
}
