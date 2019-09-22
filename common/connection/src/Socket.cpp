#include "connection/Socket.hpp"
#include <iostream>

namespace connection
{
Socket::Socket() : ioContext{}, resolver{ioContext}, socket{ioContext} {}

void Socket::connect(const boost::asio::ip::address& address, const std::string& port) // listen
{
    endpoints = resolver.resolve(address.to_string(), port);
    boost::asio::async_connect(socket, endpoints, [](boost::system::error_code ec, boost::asio::ip::tcp::endpoint) {
        if (!ec)
        {
            std::cout << "Connection works properly" << std::endl;
        }
    });

    ioContext.run();
}

boost::asio::ip::tcp::socket& Socket::value()
{
    return socket;
}

void Socket::close()
{
    socket.close();
}
} // namespace connection
