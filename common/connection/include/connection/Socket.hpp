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
    void listen(const std::string& port, std::function<void(boost::asio::ip::tcp::socket&)>);
    boost::asio::ip::tcp::socket& value();
    void close();

private:
    boost::asio::io_context ioContext;

    std::optional<boost::asio::ip::tcp::socket> socket;
    std::optional<boost::asio::ip::tcp::acceptor> socketAcceptor;
};
} // namespace connection
