#include "server_app/ServerSession.hpp"
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <iostream>
#include <stdexcept>
#include "internal_types/Visitor.hpp"
#include "key_management/FakeKey.hpp"
#include "key_management/TestKey.hpp"
#include "server_app/ConcreteReceiver.hpp"

namespace server_app
{
ServerSession::ServerSession(
    tcp::socket _socket,
    std::shared_ptr<key_management::IKey> _keyHandler,
    std::shared_ptr<mouse_management::FakeMouse> _fakeMouse,
    std::shared_ptr<IReceiver> _receiver,
    std::unique_ptr<internal_types::IDeserializer> _deserializer)
    : socket(std::move(_socket))
    , keyHandler{std::move(_keyHandler)}
    , fakeMouse{std::move(_fakeMouse)}
    , receiver{std::move(_receiver)}
    , deserilizer{std::move(_deserializer)}
{
}

void ServerSession::onMouseEvent(const internal_types::MouseEvent& mouseEvent)
{
    std::visit(
        internal_types::Visitor{
            [this](internal_types::MouseMoveEvent event) {
                std::cerr << event.deltaX << std::endl;
                std::cerr << event.deltaY << std::endl;
                fakeMouse->onEvent(event);
            },
            [](internal_types::MouseScrollEvent) {

            },
            [](internal_types::MouseKeyEvent) {

            },
        },
        mouseEvent);
}

void ServerSession::onMessage(boost::system::error_code ec, std::size_t size)
{
    if (size > 0 && !ec)
    {
        try
        {
            std::visit(
                internal_types::Visitor{
                    [this](internal_types::KeyEvent keyEvent) { keyHandler->onEvent(keyEvent); },
                    [this](internal_types::MouseEvent mouseEvent) { onMouseEvent(mouseEvent); },
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
