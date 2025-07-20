cmake_minimum_required(VERSION 3.30)
project(EchoClientServerCpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(tcp_server
        src/server/TcpServer.cpp
        src/util/logger/Logger.cpp
        src/util/echo_protocol/EchoProtocol.cpp
)

add_executable(tcp_client
        src/client/TcpClient.cpp
        src/util/logger/Logger.cpp
        src/util/echo_protocol/EchoProtocol.cpp
)