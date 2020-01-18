#pragma once

#include <boost/asio.hpp>
#include <functional>
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
    boost::asio::io_context& getIoContext();

    void close();

private:
    std::optional<boost::asio::ip::tcp::socket> socket;
    boost::asio::io_context ioContext;
};
} // namespace connection
