#include "connection/Receiver.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include "internal_types/IDeserializer.hpp"

namespace connection
{
Receiver::Receiver(boost::asio::ip::tcp::socket& socket_, std::unique_ptr<internal_types::Deserializer> deserializer)
    : socket{socket_}, deserializer{std::move(deserializer)}
{
    buffer.fill(std::byte{0});
}
} // namespace connection
