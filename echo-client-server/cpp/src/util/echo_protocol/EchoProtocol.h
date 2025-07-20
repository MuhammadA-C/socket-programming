//
// Created by muhammad chambers on 7/20/25.
//

#ifndef ECHOCLIENTSERVERCPP_ECHOPROTOCOL_H
#define ECHOCLIENTSERVERCPP_ECHOPROTOCOL_H

#include <string>


class EchoProtocol {
public:
    static std::string createEchoMessage(const std::string &messageBody);

};


#endif //ECHOCLIENTSERVERCPP_ECHOPROTOCOL_H
