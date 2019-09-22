#pragma once

#include <boost/asio.hpp>
#include <optional>
#include "internal_types/Serializer.hpp"

namespace connection
{
class Socket
{
public:
    Socket();

    void connect(const boost::asio::ip::address&, const std::string& port);
    void send(internal_types::Event);
    boost::asio::ip::tcp::socket& value();
    void close();

private:
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver::results_type endpoints;
    internal_types::Serializer serializer;
};
} // namespace connection
