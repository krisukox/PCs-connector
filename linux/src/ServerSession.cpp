#include "../include/ServerSession.hpp"
#include <iostream>

ServerSession::ServerSession(tcp::socket _socket, FakeKey& _fakeKey) : socket(std::move(_socket)), fakeKey{_fakeKey} {}

void ServerSession::start()
{
    std::cout << "start" << std::endl;
    readBody();
}

void ServerSession::readBody()
{
    auto self(shared_from_this());
    boost::asio::async_read(
        socket, boost::asio::buffer(charPtr, 2), [self](boost::system::error_code ec, std::size_t /*length*/) {
            self->fakeKey.pressKey(static_cast<u_int16_t>(self->charPtr[0]));
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
