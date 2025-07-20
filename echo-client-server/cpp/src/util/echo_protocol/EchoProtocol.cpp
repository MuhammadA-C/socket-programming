//
// Created by muhammad chambers on 7/20/25.
//

#include "EchoProtocol.h"

#include <string>



std::string EchoProtocol::createEchoMessage(const std::string &messageBody) {
    const std::string ECHO_PROTOCOL = "ECHO";
    const std::string END_SYMBOL = "\r\n";
    return ECHO_PROTOCOL + " " + std::to_string(messageBody.size()) + END_SYMBOL + messageBody + END_SYMBOL;
}
