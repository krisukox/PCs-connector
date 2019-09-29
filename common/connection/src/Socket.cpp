#include "connection/Socket.hpp"
#include <iostream>

namespace connection
{
Socket::Socket() : ioContext{} {}

void Socket::connect(const boost::asio::ip::address& address, const std::string& port) // listen
{
    socket = boost::asio::ip::tcp::socket{ioContext};
    auto resolver = boost::asio::ip::tcp::resolver{ioContext};
    auto endpoint = resolver.resolve(address.to_string(), port);
    boost::asio::async_connect(
        socket.value(), endpoint, [](boost::system::error_code ec, boost::asio::ip::tcp::endpoint) {
            if (!ec)
            {
                std::cout << "Connection works properly" << std::endl;
            }
        });

    ioContext.run();
}

void Socket::listen(const std::string& port, std::function<void(boost::asio::ip::tcp::socket&)> successfullConnection)
{
    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::tcp::v4(), static_cast<unsigned short>(std::stoi(port))};
    socketAcceptor = boost::asio::ip::tcp::acceptor{ioContext, endpoint};
    socketAcceptor->async_accept(
        [this, successfullConnection](boost::system::error_code ec, boost::asio::ip::tcp::socket socket_) mutable {
            if (!ec)
            {
                socket = std::make_optional(std::move(socket_));
                successfullConnection(socket.value());
            }
        });

    ioContext.run();
}

boost::asio::ip::tcp::socket& Socket::value()
{
    if (socket)
    {
        return socket.value();
    }
    throw std::runtime_error("Socket not allocated yet");
}

void Socket::close()
{
    socket->close();
}
} // namespace connection
