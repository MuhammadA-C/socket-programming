//
// Created by muhammad chambers on 7/18/25.
//

#ifndef ECHO_CLIENT_SERVER_TCPCLIENT_H
#define ECHO_CLIENT_SERVER_TCPCLIENT_H

#include <string>


class TcpClient {
public:
    /**
     *
     */
    struct ServerResponse {
        ssize_t totalReceivedBytes;
        std::string message;
    };

    static int initialize(const char* host, const char* port, int maxPending);
    static bool setRecvTimeout(int socketFd, int seconds);
    bool sendClientRequest(int clientSocket, const std::string &request);
    ServerResponse processServerResponse(int clientSocket);

private:

};


#endif //ECHO_CLIENT_SERVER_TCPCLIENT_H
