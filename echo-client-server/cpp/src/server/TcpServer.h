#ifndef ECHO_CLIENT_SERVER_TCPSERVER_H
#define ECHO_CLIENT_SERVER_TCPSERVER_H

#include <string>

class TcpServer {
public:
    static int createServer(const char* host, const char* port, int maxPending);
    static bool setRecvTimeout(int socketFd, int seconds);
    static std::string createEchoMessage(const std::string &messageBody);

private:


};


#endif //ECHO_CLIENT_SERVER_TCPSERVER_H
