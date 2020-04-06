#include "connection/Socket.hpp"
#include <iostream>
#include <thread>

namespace connection
{
Socket::Socket() : deserializer{}, serializer{}, ioContext{}, socket{ioContext} {}

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
    startReceiving();
    socketThread = std::thread([this]() {
        std::cout << "std thread io context START" << std::endl;
        ioContext.run();
        std::cout << "std thread io context END" << std::endl;
    });
    //    handlerIoContextThread = std::thread([this]() { ioContextHandlers.run(); });
}

void Socket::startReceiving()
{
    std::cout << "socket.async_receive START" << std::endl;
    socket.async_receive(boost::asio::buffer(buffer, 5), [this](boost::system::error_code errorCode, std::size_t size) {
        if (size > 0 && !errorCode)
        {
            startReceiving();
            std::thread t(&Socket::handleReceivedData, this, buffer);
            t.detach();
        }
        else
        {
            //            unsuccessfulCallback(errorCode);
        }
    });
    std::cout << "socket.async_receive END" << std::endl;
}

void Socket::handleReceivedData(const internal_types::Buffer& _buffer)
{
    auto decoded = deserializer.decode(_buffer);

    if (decoded)
    {
        std::cout << "Socket::handleReceivedData decoded true" << std::endl;
        std::visit([this](const auto& value) { handleReceivedType(value); }, decoded.value());
    }
    else
    {
        //        unsuccessfulCallback(boost::system::error_code());
    }
}

void Socket::close()
{
    ioContext.stop();
    socket.close();
}
} // namespace connection
