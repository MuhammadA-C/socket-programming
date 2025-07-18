//
// Created by muhammad chambers on 7/18/25.
//

#include "TcpServer.h"
#include <iostream>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>


// Move function to a different class and file
int TcpServer::createSocket(const char *host, const char *port, const int maxPending) {
    struct addrinfo addressInfoHint;
    struct addrinfo *addressInfoResult;
    struct addrinfo *ptrAddressInfo;

    memset(&addressInfoHint, 0, sizeof addressInfoHint); // ensures the struct is empty
    addressInfoHint.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    addressInfoHint.ai_socktype = SOCK_STREAM; // TCP stream sockets
    addressInfoHint.ai_flags = AI_PASSIVE; // fill in my IP for me

    int status = getaddrinfo(host, port, &addressInfoHint, &addressInfoResult);
    if (status != 0) {
        std::cerr << "getaddrinfo() error: " << gai_strerror(status) << '\n';
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
        std::cerr << "Failed creating socket and binding to port." << '\n';
        close(serverSocket);
        return -1;
    }

    // Setting server socket to listen to port for connections
    if (listen(serverSocket, maxPending) == -1) {
        std::cerr << "Failed listening for connections." << '\n';
        close(serverSocket);
        return -1;
    }
    return serverSocket;
}

int main() {
    // Note: Allow on server startup to supply port number, max pending, and help via CLI

    const int PORT = 9002;
    const int MAX_PENDING = 5;

    TcpServer server;
    int server_socket = server.createSocket(NULL, std::to_string(PORT).c_str(), MAX_PENDING);
    if (server_socket == -1) {
        std::cerr << "Server setup failed." << '\n';
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << '\n';

    // Note: Need to make server continually run forever and responds to client

    return 0;
}
