#include "connection/Socket.hpp"
#include <iostream>

class A
{
};

class B : public A
{
};

namespace connection
{
Socket::Socket() : deserializer{}, serializer{}, ioContext{}, socket{ioContext}
{
    //    A* a = new A;
    //    B* b = new B;
    //    A* c = b;
    //    //    const auto& type = typeid(internal_types::ScreenResolution);
    //    std::function<void(B*)> func = [](A*) {};
    //    //    std::function<void(A*)> func1 = func;
    //    //        dynamic_cast<std::function<void(internal_types::DecodedType)>&>(func);

    //    //    handlers.insert(typeid(internal_types::ScreenResolution), [](internal_types::ScreenResolution) {});
}

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
    ioContext.run();
}

void Socket::startReceiving()
{
    socket.async_receive(boost::asio::buffer(buffer, 5), [this](boost::system::error_code errorCode, std::size_t size) {
        if (size > 0 && !errorCode)
        {
            startReceiving();
            handleReceivedData();
        }
        else
        {
            //            unsuccessfulCallback(errorCode);
        }
    });
}

void Socket::handleReceivedData()
{
    auto decoded = deserializer.decode(buffer);

    if (decoded)
    {
        std::visit(
            internal_types::Visitor{
                [this](const auto& value) { handleReceivedData(value); },
            },
            decoded.value());
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
