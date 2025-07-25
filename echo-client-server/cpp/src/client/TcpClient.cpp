//
// Created by muhammad chambers on 7/18/25.
//

#include "TcpClient.h"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include "../util/logger/Logger.h"


// GLOBAL VARIABLES //
const int PORT = 9002;
const int MAX_PENDING = 5;

TcpClient tcpClient;


// MAIN ENTRY POINT OF APPLICATION //
int main() {
    // Note: Allow on client startup to supply port number, max pending, and help via CLI

    int clientSocket = TcpClient::initialize("localhost", std::to_string(PORT).c_str(), MAX_PENDING);
    if (clientSocket == -1) {
        Logger::log(Logger::STDERR, true, "Setup failed.");
        close(clientSocket);
        return 1;
    }

    if (!tcpClient.sendClientRequest(clientSocket, "Hello World from client")) {
        Logger::log(Logger::STDERR, true, "Failed to send message to server");
        close(clientSocket);
        return 1;
    }

    TcpClient::ServerResponse serverResponse = tcpClient.processServerResponse(clientSocket);
    Logger::log(Logger::STDOUT, true, "Servers response: " + serverResponse.message);


    // Note: Add logic to parse out the message body length to verify if received the full message


    // Clean up
    close(clientSocket);
    return 0;
}


// METHODS //

// Future Improvement: Move function to a different class and file
int TcpClient::initialize(const char *host, const char *port, int maxPending) {
    struct addrinfo addressInfoHint{};
    struct addrinfo *addressInfoResult;
    struct addrinfo *ptrAddressInfo;

    addressInfoHint.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    addressInfoHint.ai_socktype = SOCK_STREAM; // TCP stream sockets
    addressInfoHint.ai_flags = AI_PASSIVE; // fill in my IP for me

    int status = getaddrinfo(host, port, &addressInfoHint, &addressInfoResult);
    if (status != 0) {
        Logger::log(Logger::STDERR, true, "getaddrinfo() error");
        return -1;
    }

    // Loops through results from getaddrinfo() and binds port to the first one that works
    int clientSocket = -1;
    for (ptrAddressInfo = addressInfoResult; ptrAddressInfo != NULL; ptrAddressInfo = ptrAddressInfo->ai_next) {
        // Creates socket, and checks if failed
        clientSocket = socket(ptrAddressInfo->ai_family, ptrAddressInfo->ai_socktype, ptrAddressInfo->ai_protocol);
        if (clientSocket == -1) {
            continue;
        }

        // Connect to server
        if (connect(clientSocket, ptrAddressInfo->ai_addr, ptrAddressInfo->ai_addrlen) == -1) {
            close(clientSocket);
            continue;
        }
        break;
    }

    freeaddrinfo(addressInfoResult);
    if (ptrAddressInfo == NULL) {
        Logger::log(Logger::STDERR, true, "Failed creating socket and connecting to port.");
        return -1;
    }
    return clientSocket;
}

bool TcpClient::setRecvTimeout(const int socketFd, const int seconds) {
    if (socketFd < 0) {
        Logger::log(Logger::STDERR, true, "setRecvTimeout() socket file descriptor is not valid");
        return false;
    }

    const int MAX_TIMEOUT_SECONDS = 60;
    if (seconds <= 0 || seconds > MAX_TIMEOUT_SECONDS) {
        Logger::log(Logger::STDERR, true, "setRecvTimeout() seconds must be < 60 and > 0");
        return false;
    }

    struct timeval timeout{};
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    if (setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        Logger::log(Logger::STDERR, true, "setsockopt(SO_RCVTIMEO) failed.");
        return false;
    }
    Logger::log(Logger::STDOUT, true, "setsockopt(SO_RCVTIMEO) set socket timeout to " + std::to_string(seconds));
    return true;
}

bool TcpClient::sendClientRequest(const int clientSocket, const std::string &request) {
    Logger::log(Logger::STDOUT, true, "Sending the following message to the server: " + request);

    int totalBytesSent = 0;
    while (totalBytesSent < request.length()) {
        int sentBytes = send(clientSocket, request.c_str() + totalBytesSent, request.length() - totalBytesSent, 0);

        if (sentBytes < 0) {
            return false;
        }
        totalBytesSent += sentBytes;
    }
    return true;
}

TcpClient::ServerResponse TcpClient::processServerResponse(const int clientSocket) {
    int receivedBytes = 0;
    std::vector<char> tempBuffer(80);
    int totalBytesReceived = 0;
    while ((receivedBytes = recv(clientSocket, tempBuffer.data(), tempBuffer.size(), 0)) > 0) {
        totalBytesReceived += receivedBytes;
    }

    TcpClient::ServerResponse serverResponse;
    serverResponse.message = tempBuffer.data();
    serverResponse.totalReceivedBytes = totalBytesReceived;
    return serverResponse;
}
