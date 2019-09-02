#include "server_app/ServerSession.hpp"
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <iostream>
#include "key_management/FakeKey.hpp"
#include "key_management/TestKey.hpp"
#include "server_app/ConcreteReceiver.hpp"

namespace server_app
{
ServerSession::ServerSession(
    tcp::socket _socket,
    std::shared_ptr<key_management::IKey> _keyHandler,
    std::shared_ptr<IReceiver> _receiver,
    std::unique_ptr<internal_types::IDeserializer> _deserializer)
    : socket(std::move(_socket))
    , keyHandler{std::move(_keyHandler)}
    , receiver{std::move(_receiver)}
    , deserilizer{std::move(_deserializer)}
{
}

void ServerSession::onMessage(boost::system::error_code ec, std::size_t size)
{
    if (size > 0 && !ec)
    {
        try
        {
            keyHandler->onEvent(deserilizer->decode(buffer));
        }
        catch (const std::out_of_range&)
        {
            throw std::invalid_argument("Key not supported");
        }
        catch (...)
        {
            throw std::runtime_error("Unexpected exception");
        }
    }
    if (!ec)
    {
        readBody();
    }
    else
    {
        // LEAVE
    }
}

void ServerSession::start()
{
    readBody();
}

void ServerSession::readBody()
{
    receiver->asyncRead(
        socket, buffer, [this](boost::system::error_code ec, std::size_t size) { onMessage(ec, size); });
}
} // namespace server_app
