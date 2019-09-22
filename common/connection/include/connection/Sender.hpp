#pragma once

#include <boost/asio.hpp>
#include "internal_types/Serializer.hpp"

namespace connection
{
class Sender
{
public:
    Sender(boost::asio::ip::tcp::socket&);

    void send(internal_types::Event) const;

private:
    boost::asio::ip::tcp::socket& socket;
    internal_types::Serializer serializer;
};
} // namespace connection
