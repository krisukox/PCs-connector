#include "connection/Socket.hpp"
#include <iostream>
#include <thread>

namespace connection
{
Socket::Socket(std::unique_ptr<internal_types::Deserializer> _deserializer)
    : serializer{}, ioContext{}, socket{ioContext}, deserializer{std::move(_deserializer)}
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
    boost::asio::ip::tcp::acceptor acceptor(
        ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(port)));
    acceptor.accept(socket);
}

Socket::~Socket()
{
    ioContext.stop();
    socket.close();
    socketThread.join();
}

void Socket::receive(SuccessfulCallback<internal_types::DecodedType> _successfulCallback)
{
    socketThread = std::thread([this, successfulCallback = std::move(_successfulCallback)]() {
        while (receive_(successfulCallback))
            ;
    });
}

bool Socket::receive_(const SuccessfulCallback<internal_types::DecodedType>& successfulCallback)
{
    boost::system::error_code errorCode;
    socket.receive(boost::asio::buffer(buffer, 5), 0, errorCode);
    if (errorCode.failed())
    {
        return false;
    }
    auto decoded = deserializer->decode(buffer);
    if (decoded)
    {
        successfulCallback(decoded.value());
        return true;
    }
    else
    {
        return false;
    }
}
} // namespace connection
