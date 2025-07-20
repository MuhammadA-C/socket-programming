#include "TcpServer.h"
#include <iostream>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include "../util/logger/Logger.h"
#include "../util/echo_protocol/EchoProtocol.h"


// GLOBAL VARIABLES //

const int PORT = 9002;
const int BUFFER_SIZE = 1024;
const int MAX_PENDING = 5;

TcpServer tcpServer;


// MAIN ENTRY POINT OF APPLICATION //

int  main() {
    // Note: Allow on server startup to supply port number, max pending, and help via CLI

    int serverSocket = tcpServer.createServer(NULL, std::to_string(PORT).c_str(), MAX_PENDING);
    if (serverSocket == -1) {
        Logger::log(Logger::STDERR, false, "Setup failed.");
        return 1;
    }
    Logger::log(Logger::STDOUT, false, "Listening on port " + std::to_string(PORT));


    // Continually accepts client connections and sends the response echoed back
    while (true) {
        TcpServer::clientRequest clientRequest;
        clientRequest.clientFd = accept(serverSocket, NULL, NULL);
        Logger::log(Logger::STDOUT, false, "Accepting client connection");

        // Need a timeout in case the client has too long of a delay between messages/recv()
        TcpServer::setRecvTimeout(clientRequest.clientFd, 10);

        tcpServer.processRequest(clientRequest);
        Logger::log(Logger::STDOUT, false, "Received the following message from the client: " + clientRequest.message);

        tcpServer.sendServerResponse(clientRequest.clientFd, clientRequest.totalReceivedBytes, clientRequest.message);

        // Clean up socket
        close(clientRequest.clientFd);
        Logger::log(Logger::STDOUT, false, "Attempted to close client socket");
        std::cout << "\n=== Ready for next request ===" << "\n";
    }

    // Cleans up socket
    close(serverSocket);
    std::cout << "Attempted to close server socket\n";
    return 0;
}


// METHODS //

// Future Improvement: Move function to a different class and file
int TcpServer::createServer(const char *host, const char *port, int maxPending) {
    struct addrinfo addressInfoHint{};
    struct addrinfo *addressInfoResult;
    struct addrinfo *ptrAddressInfo;

    memset(&addressInfoHint, 0, sizeof addressInfoHint); // ensures the struct is empty
    addressInfoHint.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    addressInfoHint.ai_socktype = SOCK_STREAM; // TCP stream sockets
    addressInfoHint.ai_flags = AI_PASSIVE; // fill in my IP for me

    int status = getaddrinfo(host, port, &addressInfoHint, &addressInfoResult);
    if (status != 0) {
        Logger::log(Logger::STDERR, false, "getaddrinfo() failed.");
        return -1;
    }

    // Loops through results from getaddrinfo() and binds port to the first one that works
    int serverSocket = -1;
    const int REUSE_PORT = 1;
    for (ptrAddressInfo = addressInfoResult; ptrAddressInfo != NULL; ptrAddressInfo = ptrAddressInfo->ai_next) {
        // Creates socket, and checks if failed
        serverSocket = socket(ptrAddressInfo->ai_family, ptrAddressInfo->ai_socktype, ptrAddressInfo->ai_protocol);
        if (serverSocket == -1) {
            continue;
        }

        // Makes port reusable
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &REUSE_PORT, sizeof(REUSE_PORT));

        // Binding socket to port, and checks if failed
        if (bind(serverSocket, ptrAddressInfo->ai_addr, ptrAddressInfo->ai_addrlen) == -1) {
            close(serverSocket);
            continue;
        }
        break;
    }

    freeaddrinfo(addressInfoResult);
    if (ptrAddressInfo == NULL) {
        Logger::log(Logger::STDERR, false, "Failed creating socket and binding to port.");
        close(serverSocket);
        return -1;
    }

    // Setting server socket to listen to port for connections
    if (listen(serverSocket, maxPending) == -1) {
        Logger::log(Logger::STDERR, false, "Failed listening for connections.");
        close(serverSocket);
        return -1;
    }
    return serverSocket;
}

bool TcpServer::setRecvTimeout(const int socketFd, const int seconds) {
    if (socketFd < 0) {
        Logger::log(Logger::STDERR, false, "setRecvTimeout() socket file descriptor is not valid");
        return false;
    }

    const int MAX_TIMEOUT_SECONDS = 60;
    if (seconds <= 0 || seconds > MAX_TIMEOUT_SECONDS) {
        Logger::log(Logger::STDERR, false, "setRecvTimeout() seconds must be < 60 and > 0");
        return false;
    }

    struct timeval timeout{};
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    if (setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        Logger::log(Logger::STDERR, false, "setsockopt(SO_RCVTIMEO) failed");
        return false;
    }
    Logger::log(Logger::STDOUT, false, "setsockopt(SO_RCVTIMEO) set socket timeout to " + std::to_string(seconds));
    return true;
}

void TcpServer::processRequest(TcpServer::clientRequest &outClientRequest) {
    std::vector<char> buffer;
    std::vector<char> tempBuffer(BUFFER_SIZE);
    ssize_t receivedBytes = 0;

    // Recv() doesn't guarantee the message will all be sent in 1 call, so need to loop
    while ((receivedBytes = recv(outClientRequest.clientFd, tempBuffer.data(), tempBuffer.size(), 0)) > 0) {
        buffer.insert(buffer.end(), tempBuffer.begin(), tempBuffer.begin() + receivedBytes);

        /*
         * Future Improvement:
         * 1. Get length of the message the client is sending from the header
         * 2. Keep reading until full message size is received (or until timeout)
         */
        break;
    }

    // Filling in data for output
    std::string clientMessage(buffer.begin(), buffer.end());
    outClientRequest.message = clientMessage;
    outClientRequest.totalReceivedBytes = receivedBytes;
}

void TcpServer::sendServerResponse(const int clientFd, const ssize_t receivedBytes, const std::string &clientMessage) {
    // Future Improvement: Change send() to a loop since 1 send() might not be enough

    // Send error response
    if (receivedBytes == -1) {
        const std::string response = EchoProtocol::createEchoMessage("Failed to receive message");
        Logger::log(Logger::STDOUT, false, "Responded back with: " + response);
        send(clientFd, response.c_str(), response.size(), 0);
        return;
    }

    // Echo back message
    const std::string response = EchoProtocol::createEchoMessage(clientMessage);
    Logger::log(Logger::STDOUT, false, "Responded back with: " + response);
    send(clientFd, response.c_str(), response.size(), 0);
}

