#pragma once

#include <boost/asio.hpp>
#include <type_traits>
#include "connection/IReceiver.hpp"
#include "internal_types/CommonTypes.hpp"
#include "internal_types/Deserializer.hpp"
#include "internal_types/Visitor.hpp"
#include "memory"

namespace connection
{
class Receiver
{
public:
    Receiver(boost::asio::ip::tcp::socket&, std::unique_ptr<internal_types::Deserializer>);

    //    void receive(SuccessfulCallback, UnsuccessfulCallback) override;
    template <class T>
    void receive(std::function<void(T)> successfulCallback, UnsuccessfulCallback unsuccessfulCallback)
    {
        socket.async_receive(
            boost::asio::buffer(buffer, 5),
            [this, successfulCallback, unsuccessfulCallback](boost::system::error_code errorCode, std::size_t size) {
                if (size > 0 && !errorCode)
                {
                    auto decoded = deserializer->decode(buffer);

                    if (decoded)
                    {
                        std::visit(
                            internal_types::Visitor{
                                [successfulCallback](const T& value) { successfulCallback(value); },
                                [unsuccessfulCallback, errorCode](const auto&) { unsuccessfulCallback(errorCode); },
                            },
                            decoded.value());
                    }
                    else
                    {
                        unsuccessfulCallback(errorCode);
                    }
                }
                else
                {
                    unsuccessfulCallback(errorCode);
                }
            });
    }

private:
    boost::asio::ip::tcp::socket& socket;
    std::unique_ptr<internal_types::Deserializer> deserializer;

    internal_types::Buffer buffer;
};
} // namespace connection
