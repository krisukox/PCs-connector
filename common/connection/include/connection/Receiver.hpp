#pragma once

#include <boost/asio.hpp>
#include <boost/asio/socket_base.hpp>
#include <iostream>
#include <memory>
#include <type_traits>
#include "Deserializer.hpp"
#include "connection/IReceiver.hpp"
#include "internal_types/CommonTypes.hpp"
#include "internal_types/Visitor.hpp"

namespace connection
{
class Receiver
{
public:
    template <class T>
    using SuccessfulCallback = std::function<void(T)>;

    using UnsuccessfulCallback = std::function<void(boost::system::error_code)>;

    Receiver(boost::asio::ip::tcp::socket&, std::unique_ptr<internal_types::Deserializer>);

    template <class T>
    void receive(SuccessfulCallback<T> successfulCallback, UnsuccessfulCallback unsuccessfulCallback)
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

    template <class T>
    void synchronizedReceive(SuccessfulCallback<T> successfulCallback, UnsuccessfulCallback unsuccessfulCallback)
    {
        if (socket.receive(boost::asio::buffer(buffer, 5)) != 5)
        {
            std::cout << "NOT EQUAL 5!!!!!!!!!" << std::endl;
        }
        auto decoded = deserializer->decode(buffer);

        if (decoded)
        {
            std::visit(
                internal_types::Visitor{
                    [successfulCallback](const T& value) { successfulCallback(value); },
                    [unsuccessfulCallback](const auto&) { /*check if decoded value type is converted to T type*/ },
                },
                decoded.value());
        }
        else
        {
            std::cout << "UNABLE TO DECODE" << std::endl;
        }
    }

private:
    boost::asio::ip::tcp::socket& socket;
    std::unique_ptr<internal_types::Deserializer> deserializer;

    internal_types::Buffer buffer;
};
} // namespace connection
