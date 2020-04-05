#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <optional>
#include "Deserializer.hpp"
#include "internal_types/Serializer.hpp"
#include "internal_types/Visitor.hpp"

namespace connection
{
class Socket
{
public:
    template <class T>
    using SuccessfulCallback = std::function<void(T)>;
    using UnsuccessfulCallback = std::function<void(boost::system::error_code)>;

    Socket();

    Socket(const boost::asio::ip::address&, const std::string& port);
    Socket(const std::string& port);

    void start();
    void close();

    template <class T>
    void receive(SuccessfulCallback<T> successfulCallback, UnsuccessfulCallback unsuccessfulCallback)
    {
        //        boost::asio::async_read(
        //            socket,
        //            boost::asio::buffer(buffer, 5),
        //            [this, successfulCallback, unsuccessfulCallback](boost::system::error_code errorCode, std::size_t
        //            size) {
        //                std::cout << "void receive" << std::endl;
        //                if (size > 0 && !errorCode)
        //                {
        //                    handleReceivedData(successfulCallback, unsuccessfulCallback);
        //                }
        //                else
        //                {
        //                    unsuccessfulCallback(errorCode);
        //                }
        //            });
        socket.async_receive(
            boost::asio::buffer(buffer, 5),
            [this, successfulCallback, unsuccessfulCallback](boost::system::error_code errorCode, std::size_t size) {
                std::cout << "void receive" << std::endl;
                if (size > 0 && !errorCode)
                {
                    handleReceivedData(successfulCallback, unsuccessfulCallback);
                }
                else
                {
                    unsuccessfulCallback(errorCode);
                }
            });
    }

    template <class T>
    void synchronizedReceive(SuccessfulCallback<T> successfulCallback, UnsuccessfulCallback unsuccessfulCallback)
    {
        if (socket.receive(boost::asio::buffer(buffer, 5)) != 5)
        {
            unsuccessfulCallback(boost::system::error_code());
        }
        handleReceivedData(successfulCallback, unsuccessfulCallback);
        //        ioContext.stop();
    }

    template <class T>
    void send(T msgToSend)
    {
        boost::asio::async_write(
            socket,
            boost::asio::buffer(serializer.encode<T>(msgToSend), 5),
            [this](boost::system::error_code ec, std::size_t) {
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

private:
    template <class T>
    void handleReceivedData(SuccessfulCallback<T> successfulCallback, UnsuccessfulCallback unsuccessfulCallback)
    {
        auto decoded = deserializer.decode(buffer);

        if (decoded)
        {
            std::visit(
                internal_types::Visitor{
                    [successfulCallback](const T& value) { successfulCallback(value); },
                    [unsuccessfulCallback](const auto&) {
                        // TODO check if decoded value type is convertable to T type
                        unsuccessfulCallback(boost::system::error_code());
                    },
                },
                decoded.value());
        }
        else
        {
            unsuccessfulCallback(boost::system::error_code());
        }
    }

    internal_types::Deserializer deserializer;
    internal_types::Serializer serializer;
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::socket socket;

    internal_types::Buffer buffer;
};
} // namespace connection
