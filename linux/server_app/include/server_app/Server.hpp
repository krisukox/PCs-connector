#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <memory>
#include "ServerSession.hpp"

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
    Server(boost::asio::io_context&, const tcp::endpoint&, std::shared_ptr<key_management::IKey>);

private:
    void do_accept(std::shared_ptr<key_management::IKey>);

    std::optional<ServerSession> serverSession;
    tcp::acceptor socketAcceptor;
};
} // namespace server_app
