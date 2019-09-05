#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <memory>
#include "ServerSession.hpp"

namespace key_management
{
class IKey;
class IMouse;
} // namespace key_management

namespace internal_types
{
class IDeserializer;
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
        std::shared_ptr<mouse_management::IMouse>,
        std::unique_ptr<internal_types::IDeserializer>);

private:
    void do_accept(
        std::shared_ptr<key_management::IKey>,
        std::shared_ptr<mouse_management::IMouse>,
        std::unique_ptr<internal_types::IDeserializer>);

    std::unique_ptr<ServerSession> serverSession;
    tcp::acceptor socketAcceptor;
};
} // namespace server_app
