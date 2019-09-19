#include "connection/Socket.hpp"
#include <iostream>

namespace connection
{
Socket::Socket() : ioContext{}, resolver{ioContext}, socket{ioContext} {}

void Socket::connect(const boost::asio::ip::address& address, const std::string& port)
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

void Socket::send(internal_types::Event event)
{
    boost::asio::async_write(
        socket, boost::asio::buffer(serializer.encode(event), 5), [this](boost::system::error_code ec, std::size_t) {
            if (!ec)
            {
                // LATER CHECK IF VECTOR OF MSGS HAS ANY MSGS LEFT
            }
            else
            {
                socket.close();
            }
        });
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
