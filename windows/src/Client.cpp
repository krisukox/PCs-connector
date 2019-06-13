#include "Client.hpp"

#include <stdexcept>
#include <string>


Client::Client()
{
    initializeWinSock();

    createSocket();

    connctToServer();

    auto callback = [this](unsigned asciiCode){
        char sendBuf[] = {static_cast<char>(asciiCode)};
        send(sock, sendBuf, 1, 0);
    };

    keyboard = std::make_unique<Keyboard>(callback);
}

void Client::initializeWinSock()
{
    WORD version = MAKEWORD(2, 2);
    int wsResult = WSAStartup(version, &winSockData);
    if (wsResult != 0)
    {
        throw std::runtime_error("Can't start Winsock, Err #" + std::to_string(wsResult));
    }
}

void Client::createSocket()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        WSACleanup();
        throw std::runtime_error("Can't create socket, Err #" + std::to_string(WSAGetLastError()));
    }
}

void Client::connctToServer()
{
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    InetPton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    int connResult = connect(sock, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("Can't connect to server, Err #" + std::to_string(WSAGetLastError()));
    }
}
