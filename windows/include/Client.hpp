#pragma once

#include <WinSock2.h>
#include <memory>
#include <string>
#include <ws2tcpip.h>

#include "Console.hpp"
#include "Keyboard.hpp"

#pragma comment(lib, "ws2_32.lib")

class Client
{
public:
    Client();

private:
    void initializeWinSock();
    void createSocket();
    void connctToServer();

    std::unique_ptr<Keyboard> keyboard;
    WSAData winSockData;
    std::string ipAddress = "192.168.0.22";
    u_short port = 54000;

    Console console{};
    SOCKET sock;
};
