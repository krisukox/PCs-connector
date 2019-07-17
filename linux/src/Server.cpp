#include "../include/Server.hpp"

chat_server::chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint, FakeKey& _fakeKey)
    : acceptor_(io_context, endpoint), fakeKey{_fakeKey}
{
    do_accept();
}

void chat_server::do_accept()
{
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec)
        {
            std::make_shared<chat_session>(std::move(socket), fakeKey)->start();
        }

        do_accept();
    });
}
