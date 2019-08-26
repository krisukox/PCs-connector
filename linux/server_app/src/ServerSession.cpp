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
    std::shared_ptr<IReceiver> _receiver)
    : socket(std::move(_socket)), keyHandler{std::move(_keyHandler)}, receiver{std::move(_receiver)}
{
}

void ServerSession::onMessage(boost::system::error_code ec, std::size_t size)
{
    if (size > 0 && !ec)
    {
        try
        {
            keyHandler->handleEvent(charPtr[0], charPtr[1]);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
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
        socket, charPtr, [this](boost::system::error_code ec, std::size_t size) { onMessage(ec, size); });
}
} // namespace server_app
