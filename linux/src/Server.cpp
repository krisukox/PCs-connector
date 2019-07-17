#include "../include/Server.hpp"
#include "../include/ServerSession.hpp"

Server::Server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint, FakeKey& _fakeKey)
    : socketAcceptor(io_context, endpoint), fakeKey{_fakeKey}
{
    do_accept();
}

void Server::do_accept()
{
    socketAcceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec)
        {
            std::make_shared<ServerSession>(std::move(socket), fakeKey)->start();
        }

        do_accept();
    });
}
