#pragma once

#include <boost/asio.hpp>
#include "internal_types/Serializer.hpp"

namespace connection
{
class Sender
{
public:
    Sender(boost::asio::ip::tcp::socket&);

    template <class T>
    void send(T msgToSend) const
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
    boost::asio::ip::tcp::socket& socket;
    internal_types::Serializer serializer;
};
} // namespace connection
