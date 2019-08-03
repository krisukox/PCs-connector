#include "../include/ServerSession.hpp"
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <iostream>

ServerSession::ServerSession(tcp::socket _socket, std::shared_ptr<IKey> _keyHandler)
    : socket(std::move(_socket)), keyHandler{std::move(_keyHandler)}
{
}

void ServerSession::start()
{
    readBody();
}

void ServerSession::readBody()
{
    auto self(shared_from_this());
    boost::asio::async_read(
        socket, boost::asio::buffer(charPtr, 2), [self](boost::system::error_code ec, std::size_t size) {
            if (size > 0 && !ec)
            {
                self->keyHandler->handleEvent(static_cast<std::uint16_t>(self->charPtr[0]));
            }
            if (!ec)
            {
                self->readBody();
            }
            else
            {
                // LEAVE
            }
        });
}
