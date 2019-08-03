#include "../include/Server.hpp"
#include <iostream>
#include "../include/FakeKey.hpp"
#include "../include/TestKey.hpp"

Server::Server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint, std::shared_ptr<IKey> keyHander)
    : socketAcceptor(io_context, endpoint)
{
    do_accept(std::move(keyHander));
}

void Server::do_accept(std::shared_ptr<IKey> keyHander)
{
    socketAcceptor.async_accept([keyHander = std::move(keyHander)](boost::system::error_code ec, tcp::socket socket) {
        if (!ec)
        {
            std::make_shared<ServerSession>(std::move(socket), std::move(keyHander))->start();
        }

        //        do_accept();
    });
}
