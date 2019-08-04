#include "server_app/Server.hpp"
#include <iostream>
#include "key_management/FakeKey.hpp"
#include "key_management/TestKey.hpp"

namespace server_app
{
Server::Server(
    boost::asio::io_context& io_context,
    const tcp::endpoint& endpoint,
    std::shared_ptr<key_management::IKey> keyHander)
    : socketAcceptor(io_context, endpoint)
{
    do_accept(std::move(keyHander));
}

void Server::do_accept(std::shared_ptr<key_management::IKey> keyHander)
{
    socketAcceptor.async_accept([keyHander = std::move(keyHander)](boost::system::error_code ec, tcp::socket socket) {
        if (!ec)
        {
            std::make_shared<ServerSession>(std::move(socket), std::move(keyHander))->start();
        }
    });
}
} // namespace server_app
