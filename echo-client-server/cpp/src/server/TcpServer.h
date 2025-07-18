//
// Created by muhammad chambers on 7/18/25.
//

#ifndef ECHO_CLIENT_SERVER_TCPSERVER_H
#define ECHO_CLIENT_SERVER_TCPSERVER_H


class TcpServer {
public:
    int createSocket(const char* host, const char* port, int maxPending);

private:


};


#endif //ECHO_CLIENT_SERVER_TCPSERVER_H
