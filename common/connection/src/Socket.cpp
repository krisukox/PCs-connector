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
    std::cout << "Socket::Socket 11" << std::endl;
    //    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::tcp::v4(), static_cast<unsigned
    //    short>(std::stoi(port))}; auto socketAcceptor = boost::asio::ip::tcp::acceptor{ioContext, endpoint};
    //    socketAcceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket_) mutable
    //    {
    //        if (!ec)
    //        {
    //            socket = std::move(socket_);
    //            return;
    //        }
    //        std::cerr << "NOT SUCCESSFUL" << std::endl;
    //    });
    //    std::cout << "Socket::Socket 22" << std::endl;
    //    ioContext.run();
    //    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), std::stoi(port));
    //    boost::asio::ip::tcp::acceptor acceptor(ioContext);
    //    acceptor.accept(socket, endpoint);
    //    std::cout << "Socket::Socket 22" << std::endl;

    boost::asio::ip::tcp::acceptor acceptor(
        ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(port)));
    //    //    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), std::stoi(port));
    std::cout << "Socket::Socket 11" << std::endl;
    acceptor.accept(socket);
    std::cout << "Socket::Socket 22" << std::endl;
}

Socket::~Socket()
{
    ioContext.stop();
    socket.close();
    socketThread.join();
}

void Socket::start()
{
    //    startReceiving();
    socketThread = std::thread([this]() {
        std::cout << "START RECEIVE 11" << std::endl;
        //        ioContext.run();
        while (true)
            startReceiving();
        std::cout << "START RECEIVE 22" << std::endl;
    });
}

void Socket::startReceiving()
{
    //    std::cout << "startReceiving1" << std::endl;
    //    boost::system::error_code errorCode;
    //    socket.receive(boost::asio::buffer(buffer, 5), 0, errorCode);
    //    //    socket.async_receive(boost::asio::buffer(buffer, 5), [this](boost::system::error_code errorCode,
    //    std::size_t
    //    //    size) {
    //    //        if (size > 0 && !errorCode)
    //    //        {
    //    std::cout << "startReceiving2" << std::endl;
    //    msgDispatcher.handleReceivedData(buffer);
    //    //    startReceiving();
    //    std::cout << "startReceiving3" << std::endl;
    //    //            std::thread t(&MsgDispatcher::handleReceivedData, &msgDispatcher, buffer);
    //    //            t.detach();
    //    //        }
    //    //        else
    //    //        {
    //    //            //            unsuccessfulCallback(errorCode);
    //    //        }
    //    //    });
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
