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
    std::shared_ptr<key_management::IKey> keyHander,
    std::unique_ptr<internal_types::Deserializer> deserilizer)
    : socketAcceptor(io_context, endpoint)
{
    do_accept(std::move(keyHander), std::move(deserilizer));
}

void Server::do_accept(
    std::shared_ptr<key_management::IKey> keyHander,
    std::unique_ptr<internal_types::Deserializer> deserilizer)
{
    //    std::unique_ptr<internal_types::Deserializer> deserializer__{std::move(std::move(deserilizer))};
    socketAcceptor.async_accept([keyHander, this, deserilizer = std::move(deserilizer)](
                                    boost::system::error_code ec, tcp::socket socket) mutable {
        if (!ec)
        {
            serverSession = std::make_unique<ServerSession>(
                std::move(socket), std::move(keyHander), std::make_unique<ConcreteReceiver>(), std::move(deserilizer));
            serverSession->start();
        }
    });
}
} // namespace server_app
