//
// Created by muhammad chambers on 7/20/25.
//

#ifndef ECHOCLIENTSERVERCPP_LOGGER_H
#define ECHOCLIENTSERVERCPP_LOGGER_H

#include <string>


class Logger {
public:
    enum LogLevel {
        STDOUT,
        STDERR
    };


    /**
     * Used to log messages, specifically more so for message with escape characters like for protocols that contain '\\r\\n'.
     * @param logLevel Specifies where to log the message (i.e. std::cout, std::cerr)
     * @param isClient true for client, otherwise false. Used to prepend '(Client)' or '(Server)' to the message
     * @param message string message to log
     */
    static void log(LogLevel logLevel, bool isClient, const std::string &message);
};


#endif //ECHOCLIENTSERVERCPP_LOGGER_H
