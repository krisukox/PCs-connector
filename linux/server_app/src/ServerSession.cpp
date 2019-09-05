#include "server_app/ServerSession.hpp"
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <iostream>
#include <stdexcept>
#include "internal_types/Deserializer.hpp"
#include "internal_types/Visitor.hpp"
#include "key_management/FakeKey.hpp"
#include "key_management/TestKey.hpp"
#include "mouse_management/FakeMouse.hpp"
#include "server_app/ConcreteReceiver.hpp"

namespace server_app
{
ServerSession::ServerSession(
    tcp::socket _socket,
    std::shared_ptr<key_management::IKey> _keyHandler,
    std::shared_ptr<mouse_management::IMouse> _mouseHandler,
    std::shared_ptr<IReceiver> _receiver,
    std::unique_ptr<internal_types::IDeserializer> _deserializer)
    : socket(std::move(_socket))
    , keyHandler{std::move(_keyHandler)}
    , mouseHandler{std::move(_mouseHandler)}
    , receiver{std::move(_receiver)}
    , deserilizer{std::move(_deserializer)}
{
    buffer.fill(std::byte{0});
}

void ServerSession::onMessage(const boost::system::error_code& ec, const std::size_t size)
{
    if (size > 0 && !ec)
    {
        try
        {
            std::visit(
                internal_types::Visitor{
                    [this](const internal_types::KeyEvent& keyEvent) { keyHandler->onEvent(keyEvent); },
                    [this](const internal_types::MouseEvent& mouseEvent) { mouseHandler->onEvent(mouseEvent); },
                },
                deserilizer->decode(buffer));
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
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
