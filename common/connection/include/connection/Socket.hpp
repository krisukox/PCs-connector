#pragma once

#include <boost/asio.hpp>
#include <connection/MsgDispatcher.hpp>
#include <functional>
#include <unordered_map>
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
    void stop();

    template <class T>
    void receive(SuccessfulCallback<T> successfulCallback)
    {
        msgDispatcher.registerMultiple(successfulCallback);
    }

    template <class T>
    void receiveOnce(SuccessfulCallback<T> successfulCallback)
    {
        msgDispatcher.registerOnce(successfulCallback);
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
    void startReceiving();

    internal_types::Serializer serializer;
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::socket socket;
    MsgDispatcher msgDispatcher;

    internal_types::Buffer buffer;

    std::thread socketThread;
};
} // namespace connection
