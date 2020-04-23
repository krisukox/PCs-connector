#include "connection/Socket.hpp"
#include <iostream>

namespace connection
{
Socket::Socket(std::unique_ptr<internal_types::Deserializer> _deserializer)
    : serializer{}, deserializer{std::move(_deserializer)}, ioContext{}, socket{ioContext}
{
}

Socket::Socket(
    const boost::asio::ip::address& address,
    const std::string& port,
    std::unique_ptr<internal_types::Deserializer> deserializer)
    : Socket{std::move(deserializer)}
{
    boost::asio::ip::tcp::endpoint endpoint(address, std::stoi(port));
    socket.connect(endpoint);
}

Socket::Socket(const std::string& port, std::unique_ptr<internal_types::Deserializer> deserializer)
    : Socket{std::move(deserializer)}
{
    std::cerr << "SOCKET::SOCKET11" << std::endl;
    boost::asio::ip::tcp::acceptor acceptor(
        ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(port)));
    acceptor.accept(socket);
}

Socket::~Socket()
{
    ioContext.stop();
    socket.shutdown(boost::asio::socket_base::shutdown_both);
    socket.close();
    socketThread.join();
}

void Socket::receive(SuccessfulCallback<internal_types::DecodedType> _successfulCallback)
{
    if (socketThread.joinable())
    {
        socketThread.join();
    }
    socketThread = std::thread([this, successfulCallback = std::move(_successfulCallback)]() {
        auto successfulCallbackInternal = [successfulCallback](const internal_types::DecodedType& decoded) {
            successfulCallback(decoded);
            return true;
        };
        while (receive_(successfulCallbackInternal))
            ;
    });
}
} // namespace connection
