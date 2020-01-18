#include "connection/Receiver.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include "internal_types/IDeserializer.hpp"

namespace connection
{
Receiver::Receiver(boost::asio::ip::tcp::socket& socket_, std::unique_ptr<internal_types::IDeserializer> deserializer)
    : socket{socket_}, deserializer{std::move(deserializer)}
{
    buffer.fill(std::byte{0});
}

void Receiver::receive(SuccessfulCallback successfullCallback, UnsuccessfulCallback unsuccessfullCallback)
{
    socket.async_receive(
        boost::asio::buffer(buffer, 5),
        [this, successfullCallback, unsuccessfullCallback](boost::system::error_code errorCode, std::size_t size) {
            if (size > 0 && !errorCode)
            {
                successfullCallback(deserializer->decode(buffer));
            }
            else
            {
                unsuccessfullCallback(errorCode);
            }
        });
}
} // namespace connection
