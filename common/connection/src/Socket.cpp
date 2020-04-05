#include "connection/Socket.hpp"
#include <iostream>

namespace connection
{
Socket::Socket() : deserializer{}, serializer{}, ioContext{}, socket{ioContext} {}

Socket::Socket(const boost::asio::ip::address& address, const std::string& port) : Socket()
{
    boost::asio::ip::tcp::endpoint endpoint(address, std::stoi(port));
    std::cout << "Socket::connect 11" << std::endl;
    socket.connect(endpoint /*, [successfulConnection](const boost::system::error_code&) { successfulConnection(); }*/);
    std::cout << "Socket::connect 22" << std::endl;
    //    ioContext.run();
}

Socket::Socket(const std::string& port) : Socket()
{
    boost::asio::ip::tcp::acceptor acceptor(ioContext);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), std::stoi(port));
    acceptor.accept(socket, endpoint);
    //    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::tcp::v4(), static_cast<unsigned
    //    short>(std::stoi(port))}; auto socketAcceptor = boost::asio::ip::tcp::acceptor{ioContext, endpoint};
    //    socketAcceptor.async_accept(
    //        [this, successfulConnection](boost::system::error_code ec, boost::asio::ip::tcp::socket socket_) mutable {
    //            if (!ec)
    //            {
    //                socket = std::make_optional(std::move(socket_));
    //                successfulConnection(socket.value());
    //            }
    //        });
    //    ioContext.run();
}

void Socket::start()
{
    ioContext.run();
}

void Socket::close()
{
    ioContext.stop();
    socket.close();
}
} // namespace connection
