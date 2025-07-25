#ifndef ECHO_CLIENT_SERVER_TCPSERVER_H
#define ECHO_CLIENT_SERVER_TCPSERVER_H

#include <string>



class TcpServer {
public:
    /**
     *
     */
    struct ClientRequest {
        int clientFd;
        ssize_t totalReceivedBytes;
        std::string message;
    };

    /**
     *
     * @param host
     * @param port
     * @param maxPending
     * @return
     */
    int initialize(const char* host, const char* port, int maxPending);

    /**
     *
     * @param socketFd
     * @param seconds
     * @return
     */
    static bool setRecvTimeout(int socketFd, int seconds);

    /**
     *
     * @param clientFd
     * @param receivedBytes
     * @param clientMessage
     */
    void sendServerResponse(int clientFd, ssize_t receivedBytes, const std::string &clientMessage);

    /**
     *
     * @param outClientRequest
     */
    void processRequest(TcpServer::ClientRequest &outClientRequest);

private:

};


#endif //ECHO_CLIENT_SERVER_TCPSERVER_H
