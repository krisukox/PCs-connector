#include "server_app/Server.hpp"
#include <iostream>
#include "key_management/FakeKey.hpp"
#include "key_management/TestKey.hpp"
#include "server_app/ConcreteReceiver.hpp"

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
    socketAcceptor.async_accept(
        [keyHander = std::move(keyHander), this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec)
            {
                serverSession = std::make_unique<ServerSession>(
                    std::move(socket), std::move(keyHander), std::make_unique<ConcreteReceiver>());
                serverSession->start();
            }
        });
}
} // namespace server_app
