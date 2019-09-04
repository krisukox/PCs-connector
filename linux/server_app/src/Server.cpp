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
    std::shared_ptr<mouse_management::FakeMouse> fakeMouse,
    std::unique_ptr<internal_types::Deserializer> deserilizer)
    : socketAcceptor(io_context, endpoint)
{
    do_accept(std::move(keyHander), std::move(fakeMouse), std::move(deserilizer));
}

void Server::do_accept(
    std::shared_ptr<key_management::IKey> keyHander,
    std::shared_ptr<mouse_management::FakeMouse> fakeMouse,
    std::unique_ptr<internal_types::Deserializer> deserilizer)
{
    socketAcceptor.async_accept([keyHander, fakeMouse, deserilizer = std::move(deserilizer), this](
                                    boost::system::error_code ec, tcp::socket socket) mutable {
        if (!ec)
        {
            serverSession = std::make_unique<ServerSession>(
                std::move(socket),
                std::move(keyHander),
                std::move(fakeMouse),
                std::make_unique<ConcreteReceiver>(),
                std::move(deserilizer));
            serverSession->start();
        }
    });
}
} // namespace server_app
