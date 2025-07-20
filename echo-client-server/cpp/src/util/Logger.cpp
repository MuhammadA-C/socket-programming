//
// Created by muhammad chambers on 7/20/25.
//

#include "Logger.h"

#include <iostream>
#include <unordered_map>


const std::unordered_map<char, std::string> ESCAPE_CHARACTERS({
    std::make_pair('\n', "\\n"),
    std::make_pair('\r', "\\r")
});


void Logger::log(const LogLevel logLevel, bool isClient, const std::string &message) {
    const std::string MACHINE_TYPE = isClient ? "(Client) " : "(Server) ";
    std::string newMessage;

    // Converts escape characters so they are printable to the console
    for (char i : message) {
        if (ESCAPE_CHARACTERS.contains(i)) {
            newMessage += ESCAPE_CHARACTERS.at(i);
            continue;
        }
        newMessage += i;
    }

    // Logs message to console
    if (logLevel == STDERR) {
        std::cerr << MACHINE_TYPE + newMessage << "\n";
        return;
    }
    std::cout << MACHINE_TYPE + newMessage << "\n";
}
