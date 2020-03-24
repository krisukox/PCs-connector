#include "connection/Sender.hpp"

namespace connection
{
Sender::Sender(boost::asio::ip::tcp::socket& socket_) : socket{socket_} {}
} // namespace connection
