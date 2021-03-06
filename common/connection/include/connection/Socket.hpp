#pragma once

#include <boost/asio.hpp>
#include <functional>
#include "Deserializer.hpp"
#include "internal_types/CommonTypes.hpp"
#include "internal_types/Serializer.hpp"
#include "internal_types/Visitor.hpp"

namespace connection
{
class Socket
{
public:
    template <class T>
    using SuccessfulCallback = std::function<void(T)>;

    Socket(std::unique_ptr<internal_types::Deserializer>);
    Socket(const boost::asio::ip::address&, const std::string& port, std::unique_ptr<internal_types::Deserializer>);
    Socket(const std::string& port, std::unique_ptr<internal_types::Deserializer>);

    ~Socket();

    void receive(SuccessfulCallback<internal_types::DecodedType>);

    template <class T, typename = std::enable_if<std::is_convertible<T, internal_types::DecodedType>::value>>
    void receiveOnce(const SuccessfulCallback<T>&& _successfulCallback)
    {
        std::thread thread([this, successfulCallback = std::move(_successfulCallback)]() {
            auto successfulCallbackInternal = [successfulCallback](const internal_types::DecodedType& decoded) {
                return std::visit(
                    internal_types::Visitor{
                        [successfulCallback](const T& value) {
                            successfulCallback(value);
                            return true;
                        },
                        [](const auto&) { return false; },
                    },
                    decoded);
            };
            receive_(successfulCallbackInternal);
        });
        thread.detach();
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
    using SuccessfulCallbackInternal = std::function<bool(const internal_types::DecodedType&)>;

    bool receive_(const SuccessfulCallbackInternal& successfulCallback)
    {
        boost::system::error_code errorCode;
        socket.receive(boost::asio::buffer(buffer, 5), 0, errorCode);
        if (errorCode.failed())
        {
            return false;
        }
        auto decoded = deserializer->decode(buffer);
        if (decoded)
        {
            successfulCallback(decoded.value());
            return true;
        }
        else
        {
            return false;
        }
    }

    internal_types::Serializer serializer;
    std::unique_ptr<internal_types::Deserializer> deserializer;
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::socket socket;

    internal_types::Buffer buffer;
    std::thread socketThread;
};
} // namespace connection
