#include "connection/Sender.hpp"
#include <iostream>

namespace connection
{
Sender::Sender(boost::asio::ip::tcp::socket& socket_) : socket{socket_} {}

void Sender::send(internal_types::Event event) const
{
    boost::asio::async_write(
        socket, boost::asio::buffer(serializer.encode(event), 5), [this](boost::system::error_code ec, std::size_t) {
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
} // namespace connection
