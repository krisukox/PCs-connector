#pragma once

#include <boost/asio.hpp>
#include "connection/IReceiver.hpp"
#include "internal_types/CommonTypes.hpp"
#include "memory"

namespace internal_types
{
class IDeserializer;
}

namespace connection
{
class Receiver : public IReceiver
{
public:
    Receiver(boost::asio::ip::tcp::socket&, std::unique_ptr<internal_types::IDeserializer>);

    void receive(SuccessfulCallback, UnsuccessfulCallback) override;

private:
    boost::asio::ip::tcp::socket& socket;
    std::unique_ptr<internal_types::IDeserializer> deserializer;

    internal_types::Buffer buffer;
};
} // namespace connection
