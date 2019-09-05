#include "server_app/Server.hpp"
#include "internal_types/Deserializer.hpp"
#include "server_app/ConcreteReceiver.hpp"

namespace server_app
{
Server::Server(
    boost::asio::io_context& io_context,
    const tcp::endpoint& endpoint,
    std::shared_ptr<key_management::IKey> keyHander,
    std::shared_ptr<mouse_management::IMouse> mouseHandler,
    std::unique_ptr<internal_types::IDeserializer> deserilizer)
    : socketAcceptor(io_context, endpoint)
{
    do_accept(std::move(keyHander), std::move(mouseHandler), std::move(deserilizer));
}

void Server::do_accept(
    std::shared_ptr<key_management::IKey> keyHander,
    std::shared_ptr<mouse_management::IMouse> mouseHandler,
    std::unique_ptr<internal_types::IDeserializer> deserializer)
{
    socketAcceptor.async_accept([keyHander, mouseHandler, deserializer = std::move(deserializer), this](
                                    boost::system::error_code ec, tcp::socket socket) mutable {
        if (!ec)
        {
            serverSession = std::make_unique<ServerSession>(
                std::move(socket),
                std::move(keyHander),
                std::move(mouseHandler),
                std::make_unique<ConcreteReceiver>(),
                std::move(deserializer));
            serverSession->start();
        }
    });
}
} // namespace server_app
