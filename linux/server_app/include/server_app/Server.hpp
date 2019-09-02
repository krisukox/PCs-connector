#pragma once

#include <X11/Xlib.h>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <memory>
#include "ServerSession.hpp"
#include "internal_types/Deserializer.hpp"

namespace key_management
{
class IKey;
}

namespace server_app
{
using boost::asio::ip::tcp;

class Server
{
public:
    Server(
        boost::asio::io_context&,
        const tcp::endpoint&,
        std::shared_ptr<key_management::IKey>,
        std::unique_ptr<internal_types::Deserializer>);

private:
    void do_accept(std::shared_ptr<key_management::IKey>, std::unique_ptr<internal_types::Deserializer>);

    std::unique_ptr<ServerSession> serverSession;
    tcp::acceptor socketAcceptor;
};
} // namespace server_app
