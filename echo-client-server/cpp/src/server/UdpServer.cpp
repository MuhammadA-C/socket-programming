//
// Created by muhammad chambers on 7/25/25.
//

#include "UdpServer.h"

#include "iostream"
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include "../util/logger/Logger.h"


// GLOBAL VARIABLES //

const int PORT = 9002;
const int BUFFER_SIZE = 1024;
const int MAX_PENDING = 5;


// MAIN ENTRY POINT OF APPLICATION //

int main() {
    std::cout << "Hello World from UDP Server\n";

    // 1. Socket
    // 2. Bind
    // 3. Recv From
    // 4. Send To
    // 5. Close

    struct addrinfo addrInfo{};
    struct addrinfo* addrInfoResult;

    addrInfo.ai_family = AF_UNSPEC;
    addrInfo.ai_socktype = SOCK_DGRAM;
    addrInfo.ai_flags = AI_PASSIVE;

    int s = getaddrinfo(NULL, std::to_string(PORT).c_str(), &addrInfo, &addrInfoResult);


    //
    // Create Socket

    int socketFd = socket(addrInfoResult->ai_family, addrInfoResult->ai_socktype, addrInfoResult->ai_protocol);


    //
    // Bind Socket //

    if (bind(socketFd, addrInfoResult->ai_addr, addrInfoResult->ai_addrlen) == -1) {
        Logger::log(Logger::STDERR, false, "Failed to bind");
        close(socketFd);
        return 1;
    }

    // Continually process client requests
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        std::vector<char> buffer(BUFFER_SIZE);

        //
        // Receive client request //
        int bytesReceived = recvfrom(socketFd, buffer.data(), buffer.size(), 0, (struct sockaddr *) &clientAddr, &clientAddrLen);


        //
        // Send response //
        sendto(socketFd, buffer.data(), buffer.size(), 0, (struct sockaddr *) &clientAddr, clientAddrLen);

    }

    //
    // Close Socket
    close(socketFd);
    return 0;
}