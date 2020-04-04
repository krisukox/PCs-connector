#include "connection/Socket.hpp"
#include <iostream>

namespace connection
{
Socket::Socket() : ioContext{} {}

void Socket::connect(
    const boost::asio::ip::address& address,
    const std::string& port,
    std::function<void()> successfulConnection)
{
    socket = boost::asio::ip::tcp::socket{ioContext};
    //    auto resolver = boost::asio::ip::tcp::resolver{ioContext};
    boost::asio::ip::tcp::endpoint endpoint(address, std::stoi(port));
    socket.value().async_connect(
        endpoint, [successfulConnection](const boost::system::error_code&) { successfulConnection(); });
    ioContext.run();
    //    socket.value().connect(endpoint);
}

void Socket::listen(const std::string& port, std::function<void(boost::asio::ip::tcp::socket&)> successfulConnection)
{
    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::tcp::v4(), static_cast<unsigned short>(std::stoi(port))};
    auto socketAcceptor = boost::asio::ip::tcp::acceptor{ioContext, endpoint};
    socketAcceptor.async_accept(
        [this, successfulConnection](boost::system::error_code ec, boost::asio::ip::tcp::socket socket_) mutable {
            if (!ec)
            {
                socket = std::make_optional(std::move(socket_));
                successfulConnection(socket.value());
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

boost::asio::io_context& Socket::getIoContext()
{
    return ioContext;
}

void Socket::close()
{
    ioContext.stop();
    socket->close();
}
} // namespace connection
