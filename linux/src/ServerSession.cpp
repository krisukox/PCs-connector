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
    std::cout << "READ HEADER" << std::endl;
    auto self(shared_from_this());
    boost::asio::async_read(
        socket, boost::asio::buffer(charPtr, 2), [self](boost::system::error_code ec, std::size_t /*length*/) {
            self->fakeKey.pressKey(static_cast<u_int16_t>(self->charPtr[1]));
            std::cout << "header >>" << self->charPtr[0] << self->charPtr[1] << std::endl;
            if (!ec)
            {
                std::cout << "JEST W DO_READ_BODY\n";
                self->readBody();
            }
            else
            {
                // LEAVE
            }
        });
}
