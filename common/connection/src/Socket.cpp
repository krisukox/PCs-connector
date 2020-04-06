#include "connection/Socket.hpp"
#include <iostream>
#include <thread>

namespace connection
{
Socket::Socket() : serializer{}, ioContext{}, socket{ioContext}, msgDispatcher{} {}

Socket::Socket(const boost::asio::ip::address& address, const std::string& port) : Socket()
{
    boost::asio::ip::tcp::endpoint endpoint(address, std::stoi(port));
    socket.connect(endpoint);
}

Socket::Socket(const std::string& port) : Socket()
{
    boost::asio::ip::tcp::acceptor acceptor(ioContext);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), std::stoi(port));
    acceptor.accept(socket, endpoint);
}

void Socket::start()
{
    startReceiving();
    socketThread = std::thread([this]() { ioContext.run(); });
}

void Socket::startReceiving()
{
    socket.async_receive(boost::asio::buffer(buffer, 5), [this](boost::system::error_code errorCode, std::size_t size) {
        if (size > 0 && !errorCode)
        {
            startReceiving();
            std::thread t(&MsgDispatcher::handleReceivedData, &msgDispatcher, buffer);
            t.detach();
        }
        else
        {
            //            unsuccessfulCallback(errorCode);
        }
    });
}

void Socket::stop()
{
    ioContext.stop();
    socket.close();
}
} // namespace connection
